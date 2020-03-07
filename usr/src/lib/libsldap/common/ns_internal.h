/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 1999, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2017 Nexenta Systems, Inc.  All rights reserved.
 * Copyright 2020 Joyent, Inc.
 */


#ifndef	_NS_INTERNAL_H
#define	_NS_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <thread.h>
#include <lber.h>
#include <ldap.h>
#include "ns_sldap.h"
#include "ns_cache_door.h"

/*
 * INTERNALLY USED CONSTANTS
 */

#define	MAXERROR		2000
#define	TRUE			1
#define	FALSE			0
#define	NSLDAPDIRECTORY		"/var/ldap"
#define	NSCONFIGFILE		"/var/ldap/ldap_client_file"
#define	NSCONFIGREFRESH		"/var/ldap/ldap_client_file.refresh"
#define	NSCREDFILE		"/var/ldap/ldap_client_cred"
#define	NSCREDREFRESH		"/var/ldap/ldap_client_cred.refresh"
#define	ROTORSIZE		256
#define	MASK			0377
#define	LDAPMAXHARDLOOKUPTIME	256
#define	DONOTEDIT		\
	"Do not edit this file manually; your changes will be lost." \
	"Please use ldapclient (1M) instead."
#define	MAXPORTNUMBER		65535
#define	MAXPORTNUMBER_STR	"65535"
#define	CREDFILE		0
#define	CONFIGFILE		1
#define	UIDNUMFILTER		"(&(objectclass=posixAccount)(uidnumber=%s))"
#define	UIDNUMFILTER_SSD	"(&(%%s)(uidnumber=%s))"
#define	UIDFILTER		"(&(objectclass=posixAccount)(uid=%s))"
#define	UIDFILTER_SSD		"(&(%%s)(uid=%s))"
#define	UIDDNFILTER		"(objectclass=posixAccount)"

#define	HOSTFILTER		"(&(objectclass=ipHost)(cn=%s))"
#define	HOSTFILTER_SSD		"(&(%%s)(cn=%s))"

#define	SIMPLEPAGECTRLFLAG	1
#define	VLVCTRLFLAG		2

#define	LISTPAGESIZE		1000
#define	ENUMPAGESIZE		100

#define	DEFMAX			8
#define	TOKENSEPARATOR		'='
#define	QUOTETOK		'"'
#define	SPACETOK		' '
#define	COMMATOK		','
#define	COLONTOK		':'
#define	QUESTTOK		'?'
#define	SEMITOK			';'
#define	TABTOK			'\t'
#define	OPARATOK		'('
#define	CPARATOK		')'
#define	BSLTOK			'\\'
#define	DOORLINESEP		"\07"
#define	DOORLINESEP_CHR		0x7
#define	COMMASEP		", "
#define	SPACESEP		" "
#define	SEMISEP			";"
#define	COLONSEP		":"
#define	COLSPSEP		": "
#define	EQUALSEP		"="
#define	EQUSPSEP		"= "
#define	LAST_VALUE		(int)NS_LDAP_HOST_CERTPATH_P
#define	BUFSIZE			BUFSIZ
#define	DEFAULTCONFIGNAME	"__default_config"
#define	EXP_DEFAULT_TTL		"43200"	/* 12 hours TTL */
#define	CRYPTMARK		"{NS1}"
#define	DOORBUFFERSIZE		8192

#define	LDIF_FMT_STR		"%s: %s"
#define	FILE_FMT_STR		"%s= %s"
#define	DOOR_FMT_STR		"%s=%s"

#define	SESSION_CACHE_INC	8
#define	CONID_OFFSET		1024
#define	NS_DEFAULT_BIND_TIMEOUT		30 /* timeout value in seconds */
#define	NS_DEFAULT_SEARCH_TIMEOUT	30 /* timeout value in seconds */

/* max rdn length in conversion routines used by __ns_ldap_addTypedEntry() */
#define	RDNSIZE			512

/*
 * special service used by ldap_cachemgr to indicate a shadow update
 * is to be done with the credential of the administrator identity
 */
#define	NS_ADMIN_SHADOW_UPDATE	"shadow__admin_update"

/* Phase 1 profile information */
#define	_PROFILE1_OBJECTCLASS	"SolarisNamingProfile"
#define	_PROFILE_CONTAINER	"profile"
#define	_PROFILE_FILTER		"(&(|(objectclass=%s)(objectclass=%s))(cn=%s))"

/* Phase 2 profile information */
#define	_PROFILE2_OBJECTCLASS		"DUAConfigProfile"

/* Common to all profiles */
#define	_P_CN			"cn"

/* Native LDAP Phase 1 Specific Profile Attributes */
#define	_P1_SERVERS			"SolarisLDAPServers"
#define	_P1_SEARCHBASEDN		"SolarisSearchBaseDN"
#define	_P1_CACHETTL			"SolarisCacheTTL"
#define	_P1_BINDDN			"SolarisBindDN"
#define	_P1_BINDPASSWORD		"SolarisBindPassword"
#define	_P1_AUTHMETHOD			"SolarisAuthMethod"
#define	_P1_TRANSPORTSECURITY		"SolarisTransportSecurity"
#define	_P1_CERTIFICATEPATH		"SolarisCertificatePath"
#define	_P1_CERTIFICATEPASSWORD		"SolarisCertificatePassword"
#define	_P1_DATASEARCHDN		"SolarisDataSearchDN"
#define	_P1_SEARCHSCOPE			"SolarisSearchScope"
#define	_P1_SEARCHTIMELIMIT		"SolarisSearchTimeLimit"
#define	_P1_PREFERREDSERVER		"SolarisPreferredServer"
#define	_P1_PREFERREDSERVERONLY		"SolarisPreferredServerOnly"
#define	_P1_SEARCHREFERRAL		"SolarisSearchReferral"
#define	_P1_BINDTIMELIMIT		"SolarisBindTimeLimit"

/* Native LDAP Phase 2 Specific Profile Attributes */
#define	_P2_PREFERREDSERVER		"preferredServerList"
#define	_P2_DEFAULTSERVER		"defaultServerList"
#define	_P2_SEARCHBASEDN		"defaultSearchBase"
#define	_P2_SEARCHSCOPE			"defaultSearchScope"
#define	_P2_AUTHMETHOD			"authenticationMethod"
#define	_P2_CREDENTIALLEVEL		"credentialLevel"
#define	_P2_SERVICESEARCHDESC		"serviceSearchDescriptor"
#define	_P2_SEARCHTIMELIMIT		"searchTimeLimit"
#define	_P2_BINDTIMELIMIT		"bindTimeLimit"
#define	_P2_FOLLOWREFERRALS		"followReferrals"
#define	_P2_PROFILETTL			"profileTTL"
#define	_P2_ATTRIBUTEMAP		"attributeMap"
#define	_P2_OBJECTCLASSMAP		"objectClassMap"
#define	_P2_SERVICECREDLEVEL		"serviceCredentialLevel"
#define	_P2_SERVICEAUTHMETHOD		"serviceAuthenticationMethod"

/* Control & SASL information from RootDSE door call */
#define	_SASLMECHANISM			"supportedSASLmechanisms"
#define	_SASLMECHANISM_LEN		23
#define	_SUPPORTEDCONTROL		"supportedControl"
#define	_SUPPORTEDCONTROL_LEN		16

#define	NS_HASH_MAX	257
#define	NS_HASH_SCHEMA_MAPPING_EXISTED	"=MAPPING EXISTED="
#define	NS_HASH_RC_SUCCESS		1
#define	NS_HASH_RC_NO_MEMORY		-1
#define	NS_HASH_RC_CONFIG_ERROR		-2
#define	NS_HASH_RC_EXISTED		-3
#define	NS_HASH_RC_SYNTAX_ERROR		-4

/* Password management related error message from iDS ldap server */
#define	NS_PWDERR_MAXTRIES		\
	"Exceed password retry limit."
#define	NS_PWDERR_EXPIRED		\
	"password expired!"
#define	NS_PWDERR_ACCT_INACTIVATED	\
	"Account inactivated. Contact system administrator."
#define	NS_PWDERR_CHANGE_NOT_ALLOW	\
	"user is not allowed to change password"
#define	NS_PWDERR_INVALID_SYNTAX	\
	"invalid password syntax"
#define	NS_PWDERR_TRIVIAL_PASSWD	\
	"Password failed triviality check"
#define	NS_PWDERR_IN_HISTORY	\
	"password in history"
#define	NS_PWDERR_WITHIN_MIN_AGE	\
	"within password minimum age"

/*
 * INTERNALLY USED MACROS
 */

void	__s_api_debug_pause(int priority, int st, const char *mesg);

#define	NULL_OR_STR(str)	(!(str) || *(str) == '\0' ? "<NULL>" : (str))

/*
 * MKERROR: builds the error structure and fills in the status and
 * the message.  The message must be a freeable (non-static) string.
 * If it fails to allocate memory for the error structure,
 * it will return the retErr.
 */
#define	MKERROR(priority, err, st, mesg, retErr) \
	if (((err) = calloc(1, sizeof (struct ns_ldap_error))) == NULL) \
		return (retErr); \
	(err)->message = mesg; \
	(err)->status = (st); \
	__s_api_debug_pause(priority, st, (err)->message);

/*
 * MKERROR_PWD_MGMT is almost the same as MKERROR
 * except that it takes two more inputs to fill in the
 * password management information part of the
 * ns_ldap_error structure pointed to by err,
 * and it does not log a syslog message.
 */
#define	MKERROR_PWD_MGMT(err, st, mesg, pwd_status, sec_until_exp, retErr) \
	if (((err) = calloc(1, sizeof (struct ns_ldap_error))) == NULL) \
		return (retErr); \
	(err)->message = mesg; \
	(err)->status = (st); \
	(err)->pwd_mgmt.status = (pwd_status); \
	(err)->pwd_mgmt.sec_until_expired = (sec_until_exp);

#ifdef DEBUG
#define	NSLDAPTRACE(variable, setequal, message) \
	if (variable > 0 || ((setequal != 0) && (variable == setequal))) { \
		char buf[BUFSIZ]; \
		(void) snprintf(buf, BUFSIZ, message); \
		(void) write(__ldap_debug_file, buf); \
	}
#endif

/*
 * INTERNAL DATA STRUCTURES
 */

/*
 * configuration entry type
 */

typedef enum {
	SERVERCONFIG	= 1,
	CLIENTCONFIG	= 2,
	CREDCONFIG	= 3
} ns_conftype_t;

/*
 * datatype of a config entry
 */

typedef enum {
	NS_UNKNOWN	= 0,
	CHARPTR		= 1,		/* Single character pointer */
	ARRAYCP		= 2,		/* comma sep array of char pointers */
	ARRAYAUTH	= 3,		/* Array of auths */
	TIMET		= 4,		/* time relative value (TTL) */
	INT		= 5,		/* single integer */
	SSDLIST		= 6,		/* service search descriptor */
	ATTRMAP		= 7,		/* attribute mapping */
	OBJMAP		= 8,		/* objectclass mapping */
	SERVLIST	= 9,		/* serverlist (SP sep array) */
	ARRAYCRED	= 10,		/* Array of credentialLevels */
	SAMLIST		= 11,		/* serviceAuthenticationMethod */
	SCLLIST		= 12		/* serviceCredentialLevel */
} ns_datatype_t;

typedef enum {
	NS_SUCCESS,
	NS_NOTFOUND,
	NS_PARSE_ERR
} ns_parse_status;

typedef enum {
	NS_DOOR_FMT	= 1,
	NS_LDIF_FMT	= 2,
	NS_FILE_FMT	= 3
} ns_strfmt_t;

/*
 * This enum reduces the number of version string compares
 * against NS_LDAP_VERSION_1 and NS_LDAP_VERSION_2
 */

typedef enum {
	NS_LDAP_V1	= 1000,
	NS_LDAP_V2	= 2000
} ns_version_t;

/*
 * enum<->string mapping construct
 */

typedef struct ns_enum_map {
	int	value;
	char	*name;
} ns_enum_map;

#define	ENUM2INT(x)		((int)(x))

#define	INT2PARAMINDEXENUM(x)	((ParamIndexType)(x))
#define	INT2SEARCHREFENUM(x)	((SearchRef_t)(x))
#define	INT2SCOPEENUM(x)	((ScopeType_t)(x))
#define	INT2AUTHENUM(x)		((AuthType_t)(x))
#define	INT2SECENUM(x)		((TlsType_t)(x))
#define	INT2PREFONLYENUM(x)	((PrefOnly_t)(x))
#define	INT2CREDLEVELENUM(x)	((CredLevel_t)(x))
#define	INT2SHADOWUPDATENUM(x)	((enableShadowUpdate_t)(x))

#define	INT2LDAPRETURN(x)	((ns_ldap_return_code)(x))
#define	INT2CONFIGRETURN(x)	((ns_ldap_config_return_code)(x))
#define	INT2PARTIALRETURN(x)	((ns_ldap_partial_return_code)(x))

/*
 * This structure maps service name to rdn components
 * for use in __ns_getDNs. It also defines the SSD-to-use
 * service for use in __s_api_get_SSDtoUse_service.
 * The idea of an SSD-to-use service is to reduce the configuration
 * complexity. For a service, which does not have its own entries in
 * the LDAP directory, SSD for it is useless, and should not be set.
 * But since this service must share the container with at least
 * one other service which does have it own entries, the SSD for
 * this other service will be shared by this service.
 * This other service is called the SSD-to-use service.
 *
 */

typedef struct ns_service_map {
	char	*service;
	char	*rdn;
	char	*SSDtoUse_service;
} ns_service_map;

/*
 * This structure contains a single mapping from:
 * service:orig -> list of mapped
 */

typedef enum {
	NS_ATTR_MAP,
	NS_OBJ_MAP
} ns_maptype_t;

typedef struct ns_mapping {
	ns_maptype_t	type;
	char		*service;
	char		*orig;
	char		**map;
} ns_mapping_t;

/*
 * The following is the list of internal libsldap configuration data
 * structures.  The configuration is populated normally once per
 * application.  The assumption is that in applications can be
 * relatively short lived (IE ls via nsswitch) so it is important to
 * keep configuration to a minimum, but keep lookups fast.
 *
 * Assumptions:
 * 1 configuration entry per domain, and almost always 1 domain
 * per app.  Hooks exist for multiple domains per app.
 *
 * Configurations are read in from client file cache or from LDAP.
 * Attribute/objectclass mappings are hashed to improve lookup
 * speed.
 */

/*
 * Hash entry types
 */
typedef enum	_ns_hashtype_t {
	NS_HASH_AMAP	= 1,		/* attr map */
	NS_HASH_RAMAP	= 2,		/* reverse attr map */
	NS_HASH_OMAP	= 3,		/* oc map */
	NS_HASH_ROMAP	= 4,		/* reverse oc map */
	NS_HASH_VOID	= 5
} ns_hashtype_t;

typedef struct ns_hash {
	ns_hashtype_t	h_type;
	ns_mapping_t	*h_map;
	struct ns_hash	*h_next;
	struct ns_hash	*h_llnext;
} ns_hash_t;

/*
 * This structure defines the format of an internal configuration
 * parameter for ns_ldap client.
 */

typedef struct ns_param {
	ns_datatype_t	ns_ptype;
	int		ns_acnt;
	union {
		char	**ppc;
		int	*pi;
		char	*pc;
		int	i;
		time_t	tm;
	} ns_pu;
} ns_param_t;

#define	ns_ppc	ns_pu.ppc
#define	ns_pi	ns_pu.pi
#define	ns_pc	ns_pu.pc
#define	ns_i	ns_pu.i
#define	ns_tm	ns_pu.tm

/*
 * This structure defines an instance of a configuration structure.
 * paramList contains the current ns_ldap parameter configuration
 * and hashTbl contain the current attribute/objectclass mappings.
 * Parameters are indexed by using the value assigned to the parameter
 * in ParamIndexType.
 */

typedef struct ns_config {
	char			*domainName;
	ns_version_t		version;
	ns_param_t		paramList[NS_LDAP_MAX_PIT_P];
	ns_hash_t		*hashTbl[NS_HASH_MAX];
	ns_hash_t		*llHead;
	ns_ldap_entry_t		*RootDSE;
	boolean_t		delete;
	mutex_t			config_mutex;
	int			nUse;
	ldap_get_chg_cookie_t	config_cookie;
} ns_config_t;

/*
 * This structure defines the mapping of the NSCONFIGFILE file
 * statements into their corresponding SolarisNamingProfile,
 * Posix Mapping LDAP attributes, and to their corresponding
 * ParamIndexType enum mapping.  THe ParamIndexType enum
 * definitions can be found in ns_ldap.h.  This structure also
 * defines the default values that are used when a value either
 * does not exist or is undefined.
 */

typedef struct ns_default_config {
	const char	*name;		/* config file parameter name */
	ParamIndexType	index;		/* config file enum index */
	ns_conftype_t	config_type;	/* CLIENT/SERVER/CREDCONFIG */
	ns_datatype_t	data_type;	/* ppc,pi,pc,int etc... */
	int		single_valued;	/* TRUE OR FALSE */
	ns_version_t	version;	/* Version # for attribute */
	const char	*profile_name;	/* profile schema attribute name */
	ns_param_t	defval;		/* config file parameter default */
	int		(*ns_verify)(ParamIndexType i,
				struct ns_default_config *def,
				ns_param_t *param,
				char *errbuf);
	ns_enum_map	*allowed;	/* allowed values */
} ns_default_config;


/*
 * This typedef enumerates all the supported authentication
 * mechanisms currently supported in this library
 */

typedef enum EnumAuthType {
	NS_LDAP_EA_NONE				= 0,
	NS_LDAP_EA_SIMPLE			= 1,
	NS_LDAP_EA_SASL_NONE			= 2,
	NS_LDAP_EA_SASL_CRAM_MD5		= 3,
	NS_LDAP_EA_SASL_DIGEST_MD5		= 4,
	NS_LDAP_EA_SASL_DIGEST_MD5_INT		= 5,
	NS_LDAP_EA_SASL_DIGEST_MD5_CONF		= 6,
	NS_LDAP_EA_SASL_EXTERNAL		= 7,
	NS_LDAP_EA_SASL_GSSAPI			= 8,
	NS_LDAP_EA_SASL_SPNEGO			= 9,	/* unsupported */
	NS_LDAP_EA_TLS_NONE			= 10,
	NS_LDAP_EA_TLS_SIMPLE			= 11,
	NS_LDAP_EA_TLS_SASL_NONE		= 12,
	NS_LDAP_EA_TLS_SASL_CRAM_MD5		= 13,
	NS_LDAP_EA_TLS_SASL_DIGEST_MD5		= 14,
	NS_LDAP_EA_TLS_SASL_DIGEST_MD5_INT	= 15,
	NS_LDAP_EA_TLS_SASL_DIGEST_MD5_CONF	= 16,
	NS_LDAP_EA_TLS_SASL_EXTERNAL		= 17,
	NS_LDAP_EA_TLS_SASL_GSSAPI		= 18,	/* unsupported */
	NS_LDAP_EA_TLS_SASL_SPNEGO		= 19	/* unsupported */
} EnumAuthType_t;


/*
 * this enum lists the various states of the search state machine
 */

typedef enum {
	INIT			= 1,
	EXIT			= 2,
	NEXT_SEARCH_DESCRIPTOR	= 3,
	GET_SESSION		= 4,
	NEXT_SESSION		= 5,
	RESTART_SESSION		= 6,
	NEXT_SEARCH		= 7,
	NEXT_VLV		= 8,
	NEXT_PAGE		= 9,
	ONE_SEARCH		= 10,
	DO_SEARCH		= 11,
	NEXT_RESULT		= 12,
	MULTI_RESULT		= 13,
	PROCESS_RESULT		= 14,
	END_PROCESS_RESULT	= 15,
	END_RESULT		= 16,
	NEXT_REFERRAL		= 17,
	GET_REFERRAL_SESSION	= 18,
	ERROR			= 19,
	LDAP_ERROR		= 20,
	GET_ACCT_MGMT_INFO	= 21,
	CLEAR_RESULTS		= 22,
	REINIT			= 23
} ns_state_t;

/*
 * this enum lists the various states of the write state machine
 */
typedef enum {
	W_INIT			= 1,
	W_EXIT			= 2,
	GET_CONNECTION		= 3,
	SELECT_OPERATION_SYNC	= 4,
	SELECT_OPERATION_ASYNC	= 5,
	DO_ADD_SYNC		= 6,
	DO_DELETE_SYNC		= 7,
	DO_MODIFY_SYNC		= 8,
	DO_ADD_ASYNC		= 9,
	DO_DELETE_ASYNC		= 10,
	DO_MODIFY_ASYNC		= 11,
	GET_RESULT_SYNC		= 12,
	GET_RESULT_ASYNC	= 13,
	PARSE_RESULT		= 14,
	GET_REFERRAL_CONNECTION	= 15,
	W_LDAP_ERROR		= 16,
	W_ERROR			= 17
} ns_write_state_t;


typedef int ConnectionID;

/*
 * Server side sort type. Orginally the server side sort
 * was set to "cn uid". This did not work with AD and
 * hence single sort attribute was odopted. We dont
 * know which server side sort will work with the
 * Directory and hence we discover which method works.
 */
typedef enum {
	SSS_UNKNOWN		= 0,
	SSS_SINGLE_ATTR		= 1,
	SSS_CN_UID_ATTRS	= 2
} ns_srvsidesort_t;

/*
 * This structure is used by ns_connect to create and manage
 * one or more ldap connections within the library.
 */
typedef struct connection {
	ConnectionID		connectionId;
	boolean_t		usedBit;	/* true if only used by */
						/* one thread and not shared */
						/* by other threads */
	pid_t			pid;		/* process id */
	char			*serverAddr;
	ns_cred_t		*auth;
	LDAP			*ld;
	thread_t		threadID;	/* thread ID using it */
	struct ns_ldap_cookie	*cookieInfo;
	char			**controls;		/* from server_info */
	char			**saslMechanisms;	/* from server_info */
} Connection;

#define	ONE_STEP			1

/*
 * This structure is for referrals processing.
 * The data are from referral URLs returned by
 * LDAP servers
 */
typedef struct ns_referral_info {
	struct ns_referral_info	*next;
	char			*refHost;
	int			refScope;
	char			*refDN;
	char			*refFilter;
} ns_referral_info_t;

struct ns_ldap_cookie;

/*
 * Batch used by __ns_ldap_list_batch_xxx API
 */
struct ns_ldap_list_batch {
	uint32_t		nactive;
	struct ns_ldap_cookie	*next_cookie;
	struct ns_ldap_cookie	*cookie_list;
};

struct ns_conn_user;
typedef struct ns_conn_user ns_conn_user_t;

/*
 * This structure used internally in searches
 */

typedef struct ns_ldap_cookie {
	/* INPUTS */
		/* server list position */

		/* service search descriptor list & position */
	ns_ldap_search_desc_t  **sdlist;
	ns_ldap_search_desc_t  **sdpos;

		/* search filter callback */
	int			use_filtercb;
	int	(*init_filter_cb)(const ns_ldap_search_desc_t *desc,
			char **realfilter, const void *userdata);

		/* user callback */
	int			use_usercb;
	int	(*callback)(const ns_ldap_entry_t *entry,
			const void *userdata);
	const void		*userdata;

	int			followRef;
	int			use_paging;
	char			*service;
	char			*i_filter;
	const char * const	*i_attr;
	const char		*i_sortattr;
	const ns_cred_t		*i_auth;
	int			i_flags;

	/* OUTPUTS */
	ns_ldap_result_t	*result;
	ns_ldap_entry_t		*nextEntry;
		/* Error data */
	int			err_rc;
	ns_ldap_error_t		*errorp;

	/* PRIVATE */
	ns_state_t		state;
	ns_state_t		new_state;
	ns_state_t		next_state;

	Connection		*conn;
#define	conn_auth_type	conn->auth->auth.type
	ConnectionID		connectionId;

	/* paging VLV/SIMPLEPAGE data */
	int			listType;
	unsigned long		index;
	LDAPControl		**p_serverctrls;
	ns_srvsidesort_t	sortTypeTry;
	int			entryCount;

	int			scope;
	char			*basedn;
	char			*filter;
	char			**attribute;

	/* RESULT PROCESSING */
	int			msgId;
	LDAPMessage		*resultMsg;

	char			**dns;
	char			*currentdn;
	int			flag;
	struct berval		*ctrlCookie;

	/* REFERRALS PROCESSING */
	/* referralinfo list & position */
	ns_referral_info_t	*reflist;
	ns_referral_info_t	*refpos;
	/* search timeout value */
	struct timeval		search_timeout;
	/* response control to hold account management information */
	LDAPControl		**resultctrl;
	/* Flag to indicate password less account management is required */
	int			nopasswd_acct_mgmt;
	int			err_from_result;
	ns_conn_user_t		*conn_user;

	/* BATCH PROCESSING */
	ns_ldap_list_batch_t	*batch;
	boolean_t		no_wait;
	boolean_t		reinit_on_retriable_err;
	int			retries;
	ns_ldap_result_t	**caller_result;
	ns_ldap_error_t		**caller_errorp;
	int			*caller_rc;
	struct ns_ldap_cookie	*next_cookie_in_batch;
} ns_ldap_cookie_t;

/*
 * This structure is part of the return value information for
 * __s_api_requestServer.  The routine that requests a new server
 * from the cache manager
 */
typedef struct ns_server_info {
	char	*server;
	char	*serverFQDN;
	char	**controls;
	char	**saslMechanisms;
} ns_server_info_t;

/*
 * sasl callback function parameters
 */
typedef struct ns_sasl_cb_param {
	char	*mech;
	char	*authid;
	char	*authzid;
	char	*passwd;
	char	*realm;
} ns_sasl_cb_param_t;

/* Multiple threads per connection variable */
extern int MTperConn;

/*
 * INTERNAL GLOBAL DEFINITIONS AND FUNCTION DECLARATIONS
 */

#ifdef DEBUG
extern int	__ldap_debug_file;
extern int	__ldap_debug_api;
extern int	__ldap_debug_ldap;
extern int	__ldap_debug_servers;
#endif

/* internal connection APIs */
void DropConnection(ConnectionID, int);
int __s_api_getServers(char *** servers, ns_ldap_error_t ** error);

int __s_get_enum_value(ns_config_t *ptr, char *value, ParamIndexType i);
char *__s_get_auth_name(ns_config_t *ptr, AuthType_t type);
char *__s_get_security_name(ns_config_t *ptr, TlsType_t type);
char *__s_get_scope_name(ns_config_t *ptr, ScopeType_t type);
char *__s_get_pref_name(PrefOnly_t type);
char *__s_get_searchref_name(ns_config_t *ptr, SearchRef_t type);
char *__s_get_shadowupdate_name(enableShadowUpdate_t type);
char *__s_get_hostcertpath(void);
void __s_api_free_sessionPool();
int __s_api_requestServer(const char *request, const char *server,
	ns_server_info_t *ret, ns_ldap_error_t **error,  const char *addrType);


/* ************ internal sldap-api functions *********** */
void	__ns_ldap_freeEntry(ns_ldap_entry_t *ep);
void	__ns_ldap_freeASearchDesc(ns_ldap_search_desc_t *);
void	__s_api_split_key_value(char *buffer, char **name, char **value);
int	__s_api_printResult(ns_ldap_result_t *);
int	__s_api_getSearchScope(int *, ns_ldap_error_t **);
int	__s_api_getDNs(char ***, const char *,
	ns_ldap_error_t **);
int	__s_api_get_search_DNs_v1(char ***, const char *,
	ns_ldap_error_t **);
int	__s_api_getConnection(const char *, const int,
	const ns_cred_t *, int *,
	Connection **, ns_ldap_error_t **, int, int, ns_conn_user_t *);
char	**__s_api_cp2dArray(char **);
void	__s_api_free2dArray(char **);

int	__s_api_isCtrlSupported(Connection *, char *);
ns_config_t *__ns_ldap_make_config(ns_ldap_result_t *result);
ns_auth_t  *__s_api_AuthEnumtoStruct(const EnumAuthType_t i);
boolean_t __s_api_peruser_proc(void);
boolean_t __s_api_nscd_proc(void);
char	*dvalue(char *);
char	*evalue(char *);
ns_ldap_error_t *__s_api_make_error(int, char *);
ns_ldap_error_t *__s_api_copy_error(ns_ldap_error_t *);

/* ************ specific 'Standalone' functions ********** */
ns_ldap_return_code __s_api_ip2hostname(char *ipaddr, char **hostname);
struct hostent *__s_api_hostname2ip(const char *name,
				    struct hostent *result,
				    char *buffer,
				    int buflen,
				    int *h_errnop);
void	__s_api_setInitMode();
void	__s_api_unsetInitMode();
int	__s_api_isStandalone(void);
int __s_api_isInitializing();
ns_ldap_return_code __s_api_findRootDSE(const char *request,
					const char *server,
					const char *addrType,
					ns_server_info_t *ret,
					ns_ldap_error_t	**error);
ns_config_t *__s_api_create_config_door_str(char *config,
				ns_ldap_error_t **errorp);

extern void	get_environment();

/* internal Param APIs */
int		__ns_ldap_setParamValue(ns_config_t *ptr,
			const ParamIndexType type,
			const void *data, ns_ldap_error_t **error);
int		__s_api_get_type(const char *value, ParamIndexType *type);
int		__s_api_get_versiontype(ns_config_t *ptr, char *value,
					ParamIndexType *type);
int		__s_api_get_profiletype(char *value, ParamIndexType *type);
void		__s_api_init_config(ns_config_t *ptr);
void		__s_api_init_config_global(ns_config_t *ptr);
ns_parse_status __s_api_crosscheck(ns_config_t *domainptr, char *errstr,
					int check_dn);
ns_config_t	*__s_api_create_config(void);
ns_config_t	*__s_api_get_default_config(void);
ns_config_t	*__s_api_get_default_config_global(void);
ns_config_t	*__s_api_loadrefresh_config();
ns_config_t	*__s_api_loadrefresh_config_global();
void		__s_api_destroy_config(ns_config_t *ptr);
int		__s_api_get_configtype(ParamIndexType type);
const char	*__s_api_get_configname(ParamIndexType type);
char		*__s_api_strValue(ns_config_t *ptr, ParamIndexType i,
			ns_strfmt_t fmt);
void		__s_api_release_config(ns_config_t *cfg);

/* internal attribute/objectclass mapping api's */
int		 __s_api_add_map2hash(ns_config_t *config,
				ns_hashtype_t type, ns_mapping_t *map);
void		__s_api_destroy_hash(ns_config_t *config);
int		__s_api_parse_map(char *cp, char **sid,
				char **origA, char ***mapA);
char		**__ns_ldap_mapAttributeList(const char *service,
				const char * const *origAttrList);
char		*__ns_ldap_mapAttribute(const char *service,
				const char *origAttr);

/* internal configuration APIs */
void		__ns_ldap_setServer(int set);
ns_ldap_error_t	*__ns_ldap_LoadConfiguration();
ns_ldap_error_t	*__ns_ldap_LoadDoorInfo(LineBuf *configinfo, char *domainname,
				ns_config_t *new, int cred_only);
ns_ldap_error_t *__ns_ldap_DumpConfiguration(char *filename);
ns_ldap_error_t	*__ns_ldap_DumpLdif(char *filename);
int		__ns_ldap_cache_ping();
ns_ldap_error_t *__ns_ldap_print_config(int);
void		__ns_ldap_default_config();
int		__ns_ldap_download(const char *, char *, char *,
				ns_ldap_error_t **);
int __ns_ldap_check_dns_preq(int foreground, int mode_verbose, int mode_quiet,
    const char *fname, ns_ldap_self_gssapi_config_t config,
    ns_ldap_error_t **errpp);

int __ns_ldap_check_gssapi_preq(int foreground, int mode_verbose,
    int mode_quiet, ns_ldap_self_gssapi_config_t config,
    ns_ldap_error_t **errpp);

int __ns_ldap_check_all_preq(int foreground, int mode_verbose, int mode_quiet,
    ns_ldap_self_gssapi_config_t config, ns_ldap_error_t **errpp);

/* internal un-exposed APIs */
ns_cred_t	*__ns_ldap_dupAuth(const ns_cred_t *authp);
boolean_t	__s_api_is_auth_matched(const ns_cred_t *auth1,
		    const ns_cred_t *auth2);
int		__s_api_get_SSD_from_SSDtoUse_service(const char *service,
			ns_ldap_search_desc_t ***SSDlist,
			ns_ldap_error_t **errorp);
int		__s_api_prepend_automountmapname(const char *service,
			ns_ldap_search_desc_t ***SSDlist,
			ns_ldap_error_t ** errorp);
int		__s_api_prepend_automountmapname_to_dn(const char *service,
			char **basedn,
			ns_ldap_error_t ** errorp);
int		__s_api_convert_automountmapname(const char *service,
			char **dn, ns_ldap_error_t ** errorp);
int		__s_api_replace_mapped_attr_in_dn(
			const char *orig_attr, const char *mapped_attr,
			const char *dn, char **new_dn);
int		__s_api_append_default_basedn(
			const char *dn,
			char **new_dn,
			int *allocated,
			ns_ldap_error_t ** errorp);
int		__s_api_removeServer(const char *server);
void		__s_api_removeBadServers(char **server);
void		__s_api_free_server_info(ns_server_info_t *sinfo);
void		__s_api_freeConnection(Connection *con);

/* internal referrals APIs */
int		__s_api_toFollowReferrals(const int flags,
			int *toFollow,
			ns_ldap_error_t **errorp);
int		__s_api_addRefInfo(ns_referral_info_t **head,
			char *url, char *baseDN, int *scope,
			char *filter, LDAP *ld);
void		__s_api_deleteRefInfo(ns_referral_info_t *head);

/* callback routine for SSD filters */
int		__s_api_merge_SSD_filter(const ns_ldap_search_desc_t *desc,
			char **realfilter,
			const void *userdata);

/* network address verification api */
int		__s_api_isipv4(char *addr);
int		__s_api_isipv6(char *addr);
int		__s_api_ishost(char *addr);

/* password management routine */
ns_ldap_passwd_status_t
		__s_api_set_passwd_status(int errnum, char *errmsg);
int		__s_api_contain_passwd_control_oid(char **oids);

/* password less account management routine */
int		__s_api_contain_account_usable_control_oid(char **oids);

/* RFC 2307 section 5.6. Get a canonical name from entry */
char		*__s_api_get_canonical_name(ns_ldap_entry_t *entry,
			ns_ldap_attr_t *attrptr, int case_ignore);

/* self/sasl/gssapi functions */
int		__s_api_sasl_bind_callback(
			LDAP		*ld,
			unsigned	flags,
			void		*defaults,
			void		*in);

int		__s_api_self_gssapi_only_get(void);

int		__print2buf(LineBuf *line, const char *toprint, char *sep);

#ifdef __cplusplus
}
#endif

#endif /* _NS_INTERNAL_H */
