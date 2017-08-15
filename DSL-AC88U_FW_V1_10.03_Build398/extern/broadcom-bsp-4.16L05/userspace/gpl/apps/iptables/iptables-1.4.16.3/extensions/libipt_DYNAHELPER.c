#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_queue.h>

#define IPT_DYNAHELPER_OPT_PROTO 0x01
#define IPT_DYNAHELPER_OPT_TIMEOUT 0x02

enum {
	O_PROTO = 0,
	O_TIMEOUT,
};

/****************************************************************************/
/* Function which prints out usage message. */
static void DYNAHELPER_help(void)
{
	printf(
"DYNAHELPER target options:\n"
" --proto protocol		Protocol name, e.g. ftp, h323, etc.\n"
" --timeout seconds		Helper's idle timeout.\n");
}

/****************************************************************************/
static const struct xt_option_entry DYNAHELPER_opts[] = {
	{.name = "proto",   .id = O_PROTO,   .type = XTTYPE_STRING},
	{.name = "timeout", .id = O_TIMEOUT, .type = XTTYPE_STRING},
	XTOPT_TABLEEND,
};

/****************************************************************************/
/* Initialize the target. */
static void DYNAHELPER_init(struct xt_entry_target *t)
{
	struct xt_dynahelper_info *info =
	    (struct xt_dynahelper_info *) t->data;

	info->tracker = NULL;
	info->timeout = DYNAHELPER_DEFAULT_TIMEOUT;
}

/****************************************************************************/
/* Function which parses command options; returns true if it
   ate an option */
static void DYNAHELPER_parse(struct xt_option_call *cb)
{
	struct xt_dynahelper_info *info = cb->data;
	char *p;

	xtables_option_parse(cb);
	switch (cb->entry->id) {
	case O_PROTO:
		if (cb->xflags & IPT_DYNAHELPER_OPT_PROTO)
			xtables_error(PARAMETER_PROBLEM,
				   "Can't specify --proto twice");

		if (strlen(cb->arg) > DYNAHELPER_MAXPROTONAMELEN)
			xtables_error(PARAMETER_PROBLEM,
				   "Maximum protocol name length %u for "
				   "--proto", DYNAHELPER_MAXPROTONAMELEN);

		if (strlen(cb->arg) != strlen(strtok(cb->arg, "\n")))
			xtables_error(PARAMETER_PROBLEM,
				   "Newlines not allowed in --proto");

		strcpy(info->proto, cb->arg);
		for (p = info->proto; *p; p++) {
			if (isupper(*p))
				*p = tolower(*p);
		}
		cb->xflags |= IPT_DYNAHELPER_OPT_PROTO;
		break;
	case O_TIMEOUT:
		if (cb->xflags & IPT_DYNAHELPER_OPT_TIMEOUT)
			xtables_error(PARAMETER_PROBLEM,
				   "Can't specify --timeout twice");

		info->timeout = strtoul(cb->arg, NULL, 0);
		cb->xflags |= IPT_DYNAHELPER_OPT_TIMEOUT;
		break;
	}
}

/****************************************************************************/
/* Final check; nothing. */
static void DYNAHELPER_fcheck(struct xt_fcheck_call *cb)
{
	if (!(cb->xflags & IPT_DYNAHELPER_OPT_PROTO))
		xtables_error(PARAMETER_PROBLEM,
			   "DYNAHELPER: must specify --proto");
}

/****************************************************************************/
/* Prints out the targinfo. */
static void DYNAHELPER_print(const void *ip, const struct xt_entry_target *target,
                       int numeric)
{
	const struct xt_dynahelper_info *info
	    = (const struct xt_dynahelper_info *) target->data;

	printf("DYNAHELPER ");
	printf("proto %s timeout %lu", info->proto, info->timeout);
}

/****************************************************************************/
/* Saves the union ipt_targinfo in parsable form to stdout. */
static void DYNAHELPER_save(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_dynahelper_info *info
	    = (const struct xt_dynahelper_info *) target->data;

	printf("--proto %s --timeout %lu", info->proto, info->timeout);
}

/****************************************************************************/
static struct xtables_target dynahelper_tg_reg = {
	.name = "DYNAHELPER",
	.version = XTABLES_VERSION,
	.family	= NFPROTO_IPV4,
	.size = XT_ALIGN(sizeof(struct xt_dynahelper_info)),
	.userspacesize = XT_ALIGN(sizeof(struct xt_dynahelper_info)),
	.help = DYNAHELPER_help,
	.init = DYNAHELPER_init,
	.x6_parse = DYNAHELPER_parse,
	.x6_fcheck = DYNAHELPER_fcheck,
	.print = DYNAHELPER_print,
	.save = DYNAHELPER_save,
	.x6_options = DYNAHELPER_opts,
};

/****************************************************************************/
void _init(void)
{
	xtables_register_target(&dynahelper_tg_reg);
}
