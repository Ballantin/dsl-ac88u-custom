//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_UAIMPPXMLDICTIONARY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_UAIMPPXMLDICTIONARY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Basic XML
extern const char* const g_pszXML_LANG;
extern const char* const g_pszXML_ID;


// pidf.xsd
extern const char* const g_pszXML_TUPLE;
extern const char* const g_pszXML_NOTE;
extern const char* const g_pszXML_ENTITY;
extern const char* const g_pszXML_STATUS;
extern const char* const g_pszXML_CONTACT;
extern const char* const g_pszXML_TIMESTAMP;
extern const char* const g_pszXML_BASIC;
extern const char* const g_pszXML_MUSTUNDERSTAND;

// pidf.caps.xsd        // prescaps.xsd
extern const char* const g_pszXML_ATTENDANT;
extern const char* const g_pszXML_INFORMATION;
extern const char* const g_pszXML_MSG_TAKER;
extern const char* const g_pszXML_PRINCIPAL;
extern const char* const g_pszXML_BUSINESS;
extern const char* const g_pszXML_PERSONAL;
extern const char* const g_pszXML_FULL;
extern const char* const g_pszXML_HALF;
extern const char* const g_pszXML_RECEIVE_ONLY;
extern const char* const g_pszXML_SEND_ONLY;
extern const char* const g_pszXML_DEVCAPS;
extern const char* const g_pszXML_SERVCAPS;
extern const char* const g_pszXML_FIXED;
extern const char* const g_pszXML_MOBILE;
extern const char* const g_pszXML_AUDIO;
extern const char* const g_pszXML_PRIORITY;
extern const char* const g_pszXML_TEXT;
extern const char* const g_pszXML_TYPE;
extern const char* const g_pszXML_VIDEO;
extern const char* const g_pszXML_EQUALS;
extern const char* const g_pszXML_VALUE;
extern const char* const g_pszXML_HIGHERTHAN;
extern const char* const g_pszXML_LOWERTHAN;
extern const char* const g_pszXML_RANGE;
extern const char* const g_pszXML_MINVALUE;
extern const char* const g_pszXML_MAXVALUE;
extern const char* const g_pszXML_SEND_ONLY;
extern const char* const g_pszXML_ACTOR;
extern const char* const g_pszXML_CLASS;
extern const char* const g_pszXML_DESCRIPTION;
extern const char* const g_pszXML_DUPLEX;
extern const char* const g_pszXML_EVENT_PACKAGES;
extern const char* const g_pszXML_EXTENSIONS;
extern const char* const g_pszXML_METHODS;
extern const char* const g_pszXML_LANGUAGES;
extern const char* const g_pszXML_SCHEMES;
extern const char* const g_pszXML_MOBILITY;
extern const char* const g_pszXML_SUPPORTED;
extern const char* const g_pszXML_NOTSUPPORTED;
extern const char* const g_pszXML_S;
extern const char* const g_pszXML_L;
extern const char* const g_pszXML_APPLICATION;
extern const char* const g_pszXML_AUTOMATA;
extern const char* const g_pszXML_CONTROL;
extern const char* const g_pszXML_DATA;
extern const char* const g_pszXML_ISFOCUS;
extern const char* const g_pszXML_MESSAGE;

// pidf.cipid.xsd
extern const char* const g_pszXML_CARD;
extern const char* const g_pszXML_DISPLAY_NAME;
extern const char* const g_pszXML_HOMEPAGE;
extern const char* const g_pszXML_ICON;
extern const char* const g_pszXML_MAP;
extern const char* const g_pszXML_SOUND;

// pidf.data-model.xsd
extern const char* const g_pszXML_PERSON;
extern const char* const g_pszXML_DEVICEID;
extern const char* const g_pszXML_DEVICE;


// pidf.rpid.xsd
extern const char* const g_pszXML_APPOINTMENT;
extern const char* const g_pszXML_AWAY;
extern const char* const g_pszXML_BREAKFAST;
extern const char* const g_pszXML_BUSY;
extern const char* const g_pszXML_DINNER;
extern const char* const g_pszXML_HOLIDAY;
extern const char* const g_pszXML_IN_TRANSIT;
extern const char* const g_pszXML_LOOKING_FOR_WORK;
extern const char* const g_pszXML_MEAL;
extern const char* const g_pszXML_MEETING;
extern const char* const g_pszXML_ON_THE_PHONE;
extern const char* const g_pszXML_OTHER;
extern const char* const g_pszXML_PERFORMANCE;
extern const char* const g_pszXML_PERMANENT_ABSENCE;
extern const char* const g_pszXML_PLAYING;
extern const char* const g_pszXML_PRESENTATION;
extern const char* const g_pszXML_SHOPPING;
extern const char* const g_pszXML_SLEEPING;
extern const char* const g_pszXML_SPECTATOR;
extern const char* const g_pszXML_STEERING;
extern const char* const g_pszXML_TRAVEL;
extern const char* const g_pszXML_TV;
extern const char* const g_pszXML_UNKNOWN;
extern const char* const g_pszXML_VACATION;
extern const char* const g_pszXML_WORKING;
extern const char* const g_pszXML_WORSHIP;
extern const char* const g_pszXML_AFRAID;
extern const char* const g_pszXML_AMAZED;
extern const char* const g_pszXML_ANGRY;
extern const char* const g_pszXML_ANNOYED;
extern const char* const g_pszXML_ANXIOUS;
extern const char* const g_pszXML_ASHAMED;
extern const char* const g_pszXML_BORED;
extern const char* const g_pszXML_BRAVE;
extern const char* const g_pszXML_CALM;
extern const char* const g_pszXML_COLD;
extern const char* const g_pszXML_CONFUSED;
extern const char* const g_pszXML_CONTENTED;
extern const char* const g_pszXML_CRANKY;
extern const char* const g_pszXML_CURIOUS;
extern const char* const g_pszXML_DEPRESSED;
extern const char* const g_pszXML_DISAPPOINTED;
extern const char* const g_pszXML_DISGUSTED;
extern const char* const g_pszXML_DISTRACTED;
extern const char* const g_pszXML_EMBARRASSED;
extern const char* const g_pszXML_EXCITED;
extern const char* const g_pszXML_FLIRTATIOUS;
extern const char* const g_pszXML_FRUSTRATED;
extern const char* const g_pszXML_GRUMPY;
extern const char* const g_pszXML_GUILTY;
extern const char* const g_pszXML_HAPPY;
extern const char* const g_pszXML_HOT;
extern const char* const g_pszXML_HUMBLED;
extern const char* const g_pszXML_HUMILIATED;
extern const char* const g_pszXML_HUNGRY;
extern const char* const g_pszXML_HURT;
extern const char* const g_pszXML_IMPRESSED;
extern const char* const g_pszXML_IN_AWE;
extern const char* const g_pszXML_IN_LOVE;
extern const char* const g_pszXML_INDIGNANT;
extern const char* const g_pszXML_INTERESTED;
extern const char* const g_pszXML_INVINCIBLE;
extern const char* const g_pszXML_JEALOUS;
extern const char* const g_pszXML_LONELY;
extern const char* const g_pszXML_MEAN;
extern const char* const g_pszXML_MOODY;
extern const char* const g_pszXML_NERVOUS;
extern const char* const g_pszXML_NEUTRAL;
extern const char* const g_pszXML_OFFENDED;
extern const char* const g_pszXML_PLAYFUL;
extern const char* const g_pszXML_PROUD;
extern const char* const g_pszXML_RELIEVED;
extern const char* const g_pszXML_REMORSEFUL;
extern const char* const g_pszXML_RESTLESS;
extern const char* const g_pszXML_SAD;
extern const char* const g_pszXML_SARCASTIC;
extern const char* const g_pszXML_SERIOUS;
extern const char* const g_pszXML_SHOCKED;
extern const char* const g_pszXML_SHY;
extern const char* const g_pszXML_SICK;
extern const char* const g_pszXML_SLEEPY;
extern const char* const g_pszXML_STRESSED;
extern const char* const g_pszXML_SURPRISED;
extern const char* const g_pszXML_THIRSTY;
extern const char* const g_pszXML_WORRIED;
extern const char* const g_pszXML_NOISY;
extern const char* const g_pszXML_OK;
extern const char* const g_pszXML_QUIET;
extern const char* const g_pszXML_DARK;
extern const char* const g_pszXML_TOOBRIGHT;
extern const char* const g_pszXML_INAPPROPRIATE;
extern const char* const g_pszXML_UNCOMFORTABLE;
extern const char* const g_pszXML_AIRCRAFT;
extern const char* const g_pszXML_AIRPORT;
extern const char* const g_pszXML_ARENA;
extern const char* const g_pszXML_AUTOMOBILE;
extern const char* const g_pszXML_BANK;
extern const char* const g_pszXML_BAR;
extern const char* const g_pszXML_BICYCLE;
extern const char* const g_pszXML_BUS;
extern const char* const g_pszXML_BUS_STATION;
extern const char* const g_pszXML_CAFE;
extern const char* const g_pszXML_CLASSROOM;
extern const char* const g_pszXML_CLUB;
extern const char* const g_pszXML_CONSTRUCTION;
extern const char* const g_pszXML_CONVENTION_CENTER;
extern const char* const g_pszXML_GOVERNMENT;
extern const char* const g_pszXML_HOSPITAL;
extern const char* const g_pszXML_HOTEL;
extern const char* const g_pszXML_INDUSTRIAL;
extern const char* const g_pszXML_LIBRARY;
extern const char* const g_pszXML_MOTORCYCLE;
extern const char* const g_pszXML_OFFICE;
extern const char* const g_pszXML_OUTDOORS;
extern const char* const g_pszXML_PARKING;
extern const char* const g_pszXML_PLACE_OF_WORSHIP;
extern const char* const g_pszXML_PRISON;
extern const char* const g_pszXML_PUBLIC;
extern const char* const g_pszXML_PUBLIC_TRANSPORT;
extern const char* const g_pszXML_RESIDENCE;
extern const char* const g_pszXML_RESTAURANT;
extern const char* const g_pszXML_SCHOOL;
extern const char* const g_pszXML_SHOPPING_ARENA;
extern const char* const g_pszXML_STADIUM;
extern const char* const g_pszXML_STORE;
extern const char* const g_pszXML_STREET;
extern const char* const g_pszXML_THEATER;
extern const char* const g_pszXML_TRAIN;
extern const char* const g_pszXML_TRAIN_STATION;
extern const char* const g_pszXML_TRUCK;
extern const char* const g_pszXML_UNDERWAY;
extern const char* const g_pszXML_WAREHOUSE;
extern const char* const g_pszXML_WATER;
extern const char* const g_pszXML_WATERCRAFT;
extern const char* const g_pszXML_ASSISTANT;
extern const char* const g_pszXML_ASSOCIATE;
extern const char* const g_pszXML_FAMILY;
extern const char* const g_pszXML_FRIEND;
extern const char* const g_pszXML_SELF;
extern const char* const g_pszXML_SUPERVISOR;
extern const char* const g_pszXML_COURIER;
extern const char* const g_pszXML_ELECTRONIC;
extern const char* const g_pszXML_FREIGHT;
extern const char* const g_pszXML_IN_PERSON;
extern const char* const g_pszXML_POSTAL;
extern const char* const g_pszXML_HOME;
extern const char* const g_pszXML_WORK;
extern const char* const g_pszXML_CONFERENCE;
extern const char* const g_pszXML_DIALOG;
extern const char* const g_pszXML_KPML;
extern const char* const g_pszXML_MESSAGE_SUMMARY;
extern const char* const g_pszXML_POC_SETTINGS;
extern const char* const g_pszXML_PRESENCE;
extern const char* const g_pszXML_PRESENCE_WINFO;
extern const char* const g_pszXML_REFER;
extern const char* const g_pszXML_REG;
extern const char* const g_pszXML_SIEMENS_RTP_STATS;
extern const char* const g_pszXML_SPIRITS_INDPS;
extern const char* const g_pszXML_SPIRITS_USER_PROF;
extern const char* const g_pszXML_WINFO;
extern const char* const g_pszXML_ACK_UP;
extern const char* const g_pszXML_BYE_UP;
extern const char* const g_pszXML_CANCEL_UP;
extern const char* const g_pszXML_INFO_UP;
extern const char* const g_pszXML_INVITE_UP;
extern const char* const g_pszXML_MESSAGE_UP;
extern const char* const g_pszXML_NOTIFY_UP;
extern const char* const g_pszXML_OPTIONS_UP;
extern const char* const g_pszXML_PING_UP;
extern const char* const g_pszXML_PRACK_UP;
extern const char* const g_pszXML_PUBLISH_UP;
extern const char* const g_pszXML_REFER_UP;
extern const char* const g_pszXML_REGISTER_UP;
extern const char* const g_pszXML_SERVICE_UP;
extern const char* const g_pszXML_SUBSCRIBE_UP;
extern const char* const g_pszXML_UPDATE_UP;
extern const char* const g_pszXML_REL100;
extern const char* const g_pszXML_ANSWERMODE;
extern const char* const g_pszXML_EARLY_SESSION;
extern const char* const g_pszXML_EVENTLIST;
extern const char* const g_pszXML_FROM_CHANGE;
extern const char* const g_pszXML_GRUU;
extern const char* const g_pszXML_HISTINFO;
extern const char* const g_pszXML_JOIN;
extern const char* const g_pszXML_NOREFERSUB;
extern const char* const g_pszXML_OUTBOUND;
extern const char* const g_pszXML_PATH;
extern const char* const g_pszXML_PRECONDITION;
extern const char* const g_pszXML_PREF;
extern const char* const g_pszXML_PRIVACY;
extern const char* const g_pszXML_REPLACES;
extern const char* const g_pszXML_RESOURCE_PRIORITY;
extern const char* const g_pszXML_SDP_ANAT;
extern const char* const g_pszXML_SEC_AGREE;
extern const char* const g_pszXML_TDIALOG;
extern const char* const g_pszXML_TIMER;
extern const char* const g_pszXML_ACTIVITIES;
extern const char* const g_pszXML_MOOD;
extern const char* const g_pszXML_PLACE_IS;
extern const char* const g_pszXML_PLACE_TYPE;
extern const char* const g_pszXML_RELATIONSHIP;
extern const char* const g_pszXML_SERVICE_CLASS;
extern const char* const g_pszXML_SPHERE;
extern const char* const g_pszXML_STATUS_ICON;
extern const char* const g_pszXML_TIME_OFFSET;
extern const char* const g_pszXML_USER_INPUT;
extern const char* const g_pszXML_IDLE_THRESHOLD;
extern const char* const g_pszXML_LAST_INPUT;

// pidf.timed-status.xsd
extern const char* const g_pszXML_TIMED_STATUS;
extern const char* const g_pszXML_FROM;
extern const char* const g_pszXML_UNTIL;

// pidf-diff.xsd
extern const char* const g_pszXML_PIDF_DIFF;
extern const char* const g_pszXML_ADD;
extern const char* const g_pszXML_REPLACE;
extern const char* const g_pszXML_REMOVE;
extern const char* const g_pszXML_POS;
extern const char* const g_pszXML_WS;
extern const char* const g_pszXML_VERSION;
extern const char* const g_pszXML_PIDF_FULL;

// common-policy.xsd
extern const char* const g_pszXML_RULESET;
extern const char* const g_pszXML_RULE;
extern const char* const g_pszXML_CONDITIONS;
extern const char* const g_pszXML_IDENTITY;
extern const char* const g_pszXML_ONE;
extern const char* const g_pszXML_MANY;
extern const char* const g_pszXML_DOMAIN;
extern const char* const g_pszXML_EXCEPT;
extern const char* const g_pszXML_VALIDITY;
extern const char* const g_pszXML_ACTIONS;
extern const char* const g_pszXML_TRANSFORMATIONS;

// pres-rules.xsd
extern const char* const g_pszXML_SERVICE_URI_SCHEME;
extern const char* const g_pszXML_OCCURRENCE_ID;
extern const char* const g_pszXML_URI_SCHEME;
extern const char* const g_pszXML_SERVICE_URI;
extern const char* const g_pszXML_ALL_SERVICES;
extern const char* const g_pszXML_PROVIDE_SERVICES;
extern const char* const g_pszXML_ALL_DEVICES;
extern const char* const g_pszXML_PROVIDE_DEVICES;
extern const char* const g_pszXML_ALL_PERSONS;
extern const char* const g_pszXML_PROVIDE_PERSONS;
extern const char* const g_pszXML_PROVIDE_ACTIVITIES;
extern const char* const g_pszXML_PROVIDE_CLASS;
extern const char* const g_pszXML_PROVIDE_DEVICEID;
extern const char* const g_pszXML_PROVIDE_MOOD;
extern const char* const g_pszXML_PROVIDE_PLACE_IS;
extern const char* const g_pszXML_PROVIDE_PLACE_TYPE;
extern const char* const g_pszXML_PROVIDE_PRIVACY;
extern const char* const g_pszXML_PROVIDE_RELATIONSHIP;
extern const char* const g_pszXML_PROVIDE_STATUS_ICON;
extern const char* const g_pszXML_PROVIDE_SPHERE;
extern const char* const g_pszXML_PROVIDE_TIME_OFFSET;
extern const char* const g_pszXML_PROVIDE_USER_INPUT;
extern const char* const g_pszXML_PROVIDE_NOTE;
extern const char* const g_pszXML_SUB_HANDLING;
extern const char* const g_pszXML_NS;
extern const char* const g_pszXML_PROVIDE_UNKNOWN_ATTRIBUTE;
extern const char* const g_pszXML_PROVIDE_ALL_ATTRIBUTE;

// common-schema.xsd
// Every elements and attributes are already defined.

// oma-pres.xsd     // oma-pids-mods.xsd
extern const char* const g_pszXML_SERVICE_DESCRIPTION;
extern const char* const g_pszXML_SERVICE_ID;
extern const char* const g_pszXML_WILLINGNESS;
extern const char* const g_pszXML_SESSION_PARTICIPATION;
extern const char* const g_pszXML_REGISTRATION_STATE;
extern const char* const g_pszXML_REGISTRATIONTYPE;
extern const char* const g_pszXML_BARRING_STATE;
extern const char* const g_pszXML_OVERRIDING_WILLINGNESS;
extern const char* const g_pszXML_NETWORK_AVAILABILITY;
extern const char* const g_pszXML_NETWORK;
extern const char* const g_pszXML_ACTIVE;
extern const char* const g_pszXML_TERMINATED;

// resource-lists.xsd
extern const char* const g_pszXML_NAME;
extern const char* const g_pszXML_LIST;
extern const char* const g_pszXML_EXTERNAL;
extern const char* const g_pszXML_ENTRY;
extern const char* const g_pszXML_ENTRY_REF;
extern const char* const g_pszXML_REF;
extern const char* const g_pszXML_ANCHOR;
extern const char* const g_pszXML_RESOURCE_LISTS;

// rls-services.xsd
extern const char* const g_pszXML_URI;
extern const char* const g_pszXML_RLS_SERVICES;
extern const char* const g_pszXML_SERVICE;
extern const char* const g_pszXML_RESOURCE_LIST;
extern const char* const g_pszXML_PACKAGES;
extern const char* const g_pszXML_PACKAGE;

// xcap-diff.xsd
extern const char* const g_pszXML_ATTRIBUTE;
extern const char* const g_pszXML_DOCUMENT;
extern const char* const g_pszXML_ELEMENT;
extern const char* const g_pszXML_EXISTS;
extern const char* const g_pszXML_NEW_ETAG;
extern const char* const g_pszXML_PREVIOUS_ETAG;
extern const char* const g_pszXML_SEL;
extern const char* const g_pszXML_XCAP_DIFF;
extern const char* const g_pszXML_XCAP_ROOT;

// additionnal namespace
extern const char* const g_pszXML_NAMESPACE_TIMED_STATUS;
extern const char* const g_pszXML_NAMESPACE_COMMON_POLICY;
extern const char* const g_pszXML_NAMESPACE_PRES_RULES;
extern const char* const g_pszXML_NAMESPACE_RESOURCE_LISTS;
extern const char* const g_pszXML_NAMESPACE_RLS_SERVICES;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the XML namespace defined in XML specification.
extern const char* const g_pszXML_NAMESPACE_XML;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the PIDF namespace defined in RFC 3863.
extern const char* const g_pszXML_NAMESPACE_PIDF;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the PIDF data model namespace defined in RFC 4479.
extern const char* const g_pszXML_NAMESPACE_PIDFDATAMODEL;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the pidf-diff namespace defined in
//  draft-ietf-simple-partial-pidf-format-10.
extern const char* const g_pszXML_NAMESPACE_PIDFDIFF;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the rpid namespace defined in RFC 4480.
extern const char* const g_pszXML_NAMESPACE_RPID;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the location-type namespace defined in RFC 4589.
extern const char* const g_pszXML_NAMESPACE_LOCATIONTYPE;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the cipid namespace defined in RFC 4482.
extern const char* const g_pszXML_NAMESPACE_CIPID;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the PIDF capabilities namespace defined in RFC
//  5196.
extern const char* const g_pszXML_NAMESPACE_PRESCAPS;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the OMA PIDF extensions namespace defined in
//  OMA-SUP-XSD_prs_pidf_omapres-V1_0_1-20061128-A.
extern const char* const g_pszXML_NAMESPACE_OMAPRES;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the xcap-diff namespace defined in
//  draft-ietf-simple-xcap-diff-09.
extern const char* const g_pszXML_NAMESPACE_XCAPDIFF;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents an invalid namespace for deprecated SIP elements
//  that have no associated namespaces (like the PING request).
extern const char* const g_pszXML_NAMESPACE_INVALID;


// Namespace prefix.
// These variables are declared here but they are not added in the dictionary.

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the timed status namespace defined
//  RFC 4481.
extern const char* const g_pszXML_NAMESPACE_TIMED_STATUS_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the XML namespace defined in the XML
//  specification.
extern const char* const g_pszXML_NAMESPACE_XML_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the PIDF namespace defined in RFC
//  3863.
extern const char* const g_pszXML_NAMESPACE_PIDF_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the PIDF data model namespace defined
//  in RFC 4479.
extern const char* const g_pszXML_NAMESPACE_PIDFDATAMODEL_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the pidf-diff namespace defined in
//  draft-ietf-simple-partial-pidf-format-10.
extern const char* const g_pszXML_NAMESPACE_PIDFDIFF_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the rpid namespace defined in RFC
//  4480.
extern const char* const g_pszXML_NAMESPACE_RPID_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the location-type namespace defined
//  in RFC 4589.
extern const char* const g_pszXML_NAMESPACE_LOCATIONTYPE_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the cipid namespace defined in RFC
//  4482.
extern const char* const g_pszXML_NAMESPACE_CIPID_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the PIDF capabilities namespace
//  defined in RFC 5196.
extern const char* const g_pszXML_NAMESPACE_PRESCAPS_PREFIX;

// <GROUP TID_PACKAGE_UAIMPP_PIDF_XML_STRUCTSENUMS>
//
// Description:
//  This constant represents the prefix of the OMA PIDF extensions namespace
//  defined in OMA-SUP-XSD_prs_pidf_omapres-V1_0_1-20061128-A.
extern const char* const g_pszXML_NAMESPACE_OMAPRES_PREFIX;

// additionnal namespace prefix.
extern const char* const g_pszXML_NAMESPACE_COMMON_POLICY_PREFIX;
extern const char* const g_pszXML_NAMESPACE_PRES_RULES_PREFIX;
extern const char* const g_pszXML_NAMESPACE_RESOURCE_LIST_PREFIX;


// Function used to initialize and finalize the dictionary.
extern mxt_result InitializeUaImppDictionary();
extern void FinalizeUaImppDictionary();

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_UAIMPPXMLDICTIONARY_H

