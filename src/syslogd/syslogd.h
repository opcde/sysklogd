// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2021  Alexey Gladkov <gladkov.alexey@gmail.com>
 *
 * This file is part of the sysklogd package, a kernel and system log daemon.
 *
 * GNU Inetutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or (at
 * your option) any later version.
 *
 * GNU Inetutils is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see `http://www.gnu.org/licenses/'.
 */
#ifndef _SYSLOGD_H_
#define _SYSLOGD_H_

#include "attribute.h"

enum input_type {
	INPUT_NONE = 0,
	INPUT_UNIX,
	INPUT_INET,
	INPUT_SIGNALFD,
};

/* values for globals.options */
enum option_flag {
	OPT_SEND_TO_ALL   = (1 << 0), /* send message to all IPv4/IPv6 addresses */
	OPT_FORK          = (1 << 1), /* don't fork - don't run in daemon mode */
	OPT_COMPRESS      = (1 << 2), /* compress repeated messages flag */
	OPT_NET_HOPS      = (1 << 3), /* can we bounce syslog messages through an
	                               * intermediate host. */
	OPT_ACCEPT_REMOTE = (1 << 4), /* receive messages that come via UDP */
	OPT_BOOT_ID       = (1 << 5)  /* include boot ID in messages */
};

struct globals {
	int family;
	int verbose;
	unsigned options;
	char *chroot_dir;           /* new server root directory */
	char *server_user;          /* user name to run server as */
	char *bind_addr;            /* bind UDP port to this interface only */
	unsigned int mark_interval; /* interval between marks in seconds */
	char **strip_domains;       /* these domains may be stripped before writing logs */
	char **local_hosts;         /* these hosts are logged with their hostname */
	const char *devlog;
	const char *config_file;
	const char *funix_dir;
	char boot_id[5];            /* Last 4 of the kernel boot ID */
};

enum log_format_type {
	LOG_FORMAT_NONE = 0,
	LOG_FORMAT_BOL,
	LOG_FORMAT_HASH,
	LOG_FORMAT_TIME,
	LOG_FORMAT_HOST,
	LOG_FORMAT_PID,
	LOG_FORMAT_UID,
	LOG_FORMAT_GID,
	LOG_FORMAT_PRI,
	LOG_FORMAT_TAG,
	LOG_FORMAT_CONTENT,
	LOG_FORMAT_MSG,
	LOG_FORMAT_EOL,
	LOG_FORMAT_COUNTS,
};

struct log_format {
	char *line;

	enum log_format_type *type; /* list of iov element types */
	struct iovec *iov;          /* log format parts and placeholders for message parts */
	size_t iov_nr;              /* number of elements in type and iov lists */

	unsigned int mask;

	struct iovec values[LOG_FORMAT_COUNTS];
};

extern int set_input(enum input_type type, const char *name, int fd);
extern void parse_arguments(int argc, char **argv, struct globals *g);

extern void logerror(const char *fmt, ...) SYSKLOGD_FORMAT((__printf__, 1, 2)) SYSKLOGD_NONNULL((1));

extern int parse_log_format(struct log_format *log_fmt, const char *s);
extern void free_log_format(struct log_format *fmt);

#endif /* _SYSLOGD_H_ */
