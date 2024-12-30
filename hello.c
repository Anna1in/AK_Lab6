
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define DEBUG
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/list.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training with parameters and list");
MODULE_LICENSE("Dual BSD/GPL");

// Parameter for repeat count
static uint repeat_count = 1;
module_param(repeat_count, uint, 0444);
MODULE_PARM_DESC(repeat_count, "Number of times to print 'Hello, world!'");

// Define a structure for the linked list
struct event_list {
    struct list_head list;
    ktime_t event_time;
};

// Declare and initialize the head of the list
static LIST_HEAD(event_head);

static int __init hello_init(void) {
    struct event_list *event;
    unsigned int i;

    if (repeat_count == 0 || (repeat_count >= 5 && repeat_count <= 10)) {
        pr_warn("Warning: repeat_count is 0 or between 5 and 10\n");
    }

    if (repeat_count > 10) {
        pr_err("Error: repeat_count is greater than 10\n");
        return -EINVAL;
    }

    for (i = 0; i < repeat_count; i++) {
        // Allocate memory for a new list element
        event = kmalloc(sizeof(*event), GFP_KERNEL);
        if (!event) {
            pr_err("Memory allocation failed\n");
            return -ENOMEM;
        }

        // Record the current time and add the element to the list
        event->event_time = ktime_get();
        list_add_tail(&event->list, &event_head);

        // Print the message
        pr_info("Hello, world! (%u)\n", i + 1);
    }

    return 0;
}

static void __exit hello_exit(void) {
	struct event_list *event, *tmp;

	pr_debug("Starting...");
    
    list_for_each_entry_safe(event, tmp, &event_head, list) {
        pr_debug("Event time: %lld ns\n", ktime_to_ns(event->event_time));
        list_del(&event->list);
        kfree(event);
	pr_debug("Goodbye, world!");
    }
    
}

module_init(hello_init);
module_exit(hello_exit);


