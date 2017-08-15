#!/usr/bin/perl

=begin copyright-private

/***********************************************************************
 *
 *  Copyright (c) 2010  Broadcom Corporation
 *  All Rights Reserved
 *
<:license-private
 *
 ************************************************************************/

=end copyright-private

=cut


use strict;
use warnings;

use lib "../../../PerlLib";
use Test::Omci;
use XML::LibXML;
use Getopt::Long;
use Data::Dumper;

my $target;
my $gencode;
my $debug = 1;

GetOptions( 'target=s', \$target, 'debug=i', \$debug, 'code', \$gencode );

my $omci = new Test::Omci(
    errors_undef => 0,
    debug        => $debug,
    trace        => 1,
    target       => $target,
);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>OntData</Entity>
    <Instance>0</Instance>
    <AttributeMask>MibDataSync</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <MibReset ar="1" ak="0">
    <Entity>OntData</Entity>
    <Instance>0</Instance>
  </MibReset>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>NumberOfParts</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part2</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part3</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part4</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part5</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part6</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part7</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part8</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part9</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part10</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part11</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part12</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part13</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part14</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>Part15</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>1</Instance>
    <AttributeMask>NumberOfParts Part1</AttributeMask>
    <Attributes>
      <NumberOfParts>1</NumberOfParts>
      <Part1>SIPNET</Part1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>NumberOfParts</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part2</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part3</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part4</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part5</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part6</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part7</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part8</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part9</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part10</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part11</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part12</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part13</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part14</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>Part15</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>2</Instance>
    <AttributeMask>NumberOfParts Part1</AttributeMask>
    <Attributes>
      <NumberOfParts>1</NumberOfParts>
      <Part1>0.0.0.0</Part1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>NumberOfParts</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part2</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part3</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part4</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part5</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part6</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part7</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part8</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part9</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part10</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part11</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part12</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part13</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part14</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>Part15</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>3</Instance>
    <AttributeMask>NumberOfParts Part1</AttributeMask>
    <Attributes>
      <NumberOfParts>1</NumberOfParts>
      <Part1>SIPNET</Part1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>NetworkAddress</Entity>
    <Instance>1</Instance>
    <Attributes>
      <SecurityPointer>65535</SecurityPointer>
      <AddressPointer>3</AddressPointer>
    </Attributes>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>NetworkAddress</Entity>
    <Instance>1</Instance>
    <AttributeMask>SecurityPointer AddressPointer</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>1</Instance>
    <Attributes>
      <ProxyServerAddressPointer>1</ProxyServerAddressPointer>
      <OutboundProxyAddressPointer>2</OutboundProxyAddressPointer>
      <PrimarySipDns>00000000</PrimarySipDns>
      <SecondarySipDns>00000000</SecondarySipDns>
      <HostPartUri>65535</HostPartUri>
      <SipRegistrar>1</SipRegistrar>
      <SoftSwitch>0000</SoftSwitch>
      <SipOptionTxControl>0</SipOptionTxControl>
      <SipUriFormat>0</SipUriFormat>
    </Attributes>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>1</Instance>
    <AttributeMask>ProxyServerAddressPointer OutboundProxyAddressPointer PrimarySipDns SecondarySipDns TcpUdpPointer SipRegExpireTime SipReregHeadStartTime HostPartUri SipStatus</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>1</Instance>
    <AttributeMask>SipRegistrar SoftSwitch</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>1</Instance>
    <AttributeMask>TcpUdpPointer</AttributeMask>
    <Attributes>
      <TcpUdpPointer>1</TcpUdpPointer>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>1</Instance>
    <AttributeMask>SipRegExpireTime</AttributeMask>
    <Attributes>
      <SipRegExpireTime>3600</SipRegExpireTime>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>1</Instance>
    <AttributeMask>SipReregHeadStartTime</AttributeMask>
    <Attributes>
      <SipReregHeadStartTime>360</SipReregHeadStartTime>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>ValidationScheme</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>Username1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>Password</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>Realm</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>ValidationScheme</AttributeMask>
    <Attributes>
      <ValidationScheme>1</ValidationScheme>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>Username1</AttributeMask>
    <Attributes>
      <Username1>9340</Username1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>1</Instance>
    <AttributeMask>Password</AttributeMask>
    <Attributes>
      <Password>Password1</Password>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>NumberOfParts</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part2</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part3</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part4</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part5</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part6</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part7</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part8</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part9</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part10</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part11</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part12</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part13</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part14</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>Part15</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>4</Instance>
    <AttributeMask>NumberOfParts Part1</AttributeMask>
    <Attributes>
      <NumberOfParts>1</NumberOfParts>
      <Part1>5551001</Part1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>1</Instance>
    <Attributes>
      <SipAgentPointer>1</SipAgentPointer>
      <UserPartAor>3</UserPartAor>
      <UsernamePassword>1</UsernamePassword>
      <VoicemailServerSipUri>65535</VoicemailServerSipUri>
      <VoicemailSubscriptionExpireTime>3600</VoicemailSubscriptionExpireTime>
      <NetworkDialPlanPointer>65535</NetworkDialPlanPointer>
      <ApplicationServicesProfilePointer>65535</ApplicationServicesProfilePointer>
      <FeatureCodePointer>65535</FeatureCodePointer>
      <PptpPointer>769</PptpPointer>
    </Attributes>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>1</Instance>
    <AttributeMask>SipAgentPointer UserPartAor</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>1</Instance>
    <AttributeMask>SipDisplayName</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>1</Instance>
    <AttributeMask>UsernamePassword VoicemailServerSipUri VoicemailSubscriptionExpireTime NetworkDialPlanPointer ApplicationServicesProfilePointer FeatureCodePointer PptpPointer ReleaseTimer RohTimer</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>IpHostConfig</Entity>
    <Instance>1</Instance>
    <AttributeMask>IpOptions IpAddress Mask Gateway</AttributeMask>
    <Attributes>
      <IpOptions>2</IpOptions>
      <IpAddress>SIPHEX</IpAddress>
      <Mask>ffffff00</Mask>
      <Gateway>GATEHEX</Gateway>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>2</Instance>
    <Attributes>
      <ProxyServerAddressPointer>1</ProxyServerAddressPointer>
      <OutboundProxyAddressPointer>2</OutboundProxyAddressPointer>
      <PrimarySipDns>00000000</PrimarySipDns>
      <SecondarySipDns>00000000</SecondarySipDns>
      <HostPartUri>65535</HostPartUri>
      <SipRegistrar>1</SipRegistrar>
      <SoftSwitch>0000</SoftSwitch>
      <SipOptionTxControl>0</SipOptionTxControl>
      <SipUriFormat>0</SipUriFormat>
    </Attributes>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>2</Instance>
    <AttributeMask>ProxyServerAddressPointer OutboundProxyAddressPointer PrimarySipDns SecondarySipDns TcpUdpPointer SipRegExpireTime SipReregHeadStartTime HostPartUri SipStatus</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>2</Instance>
    <AttributeMask>SipRegistrar SoftSwitch</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>2</Instance>
    <AttributeMask>TcpUdpPointer</AttributeMask>
    <Attributes>
      <TcpUdpPointer>1</TcpUdpPointer>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>2</Instance>
    <AttributeMask>SipRegExpireTime</AttributeMask>
    <Attributes>
      <SipRegExpireTime>3600</SipRegExpireTime>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>SipAgentConfigData</Entity>
    <Instance>2</Instance>
    <AttributeMask>SipReregHeadStartTime</AttributeMask>
    <Attributes>
      <SipReregHeadStartTime>360</SipReregHeadStartTime>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>ValidationScheme</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>Username1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>Password</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>Realm</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>ValidationScheme</AttributeMask>
    <Attributes>
      <ValidationScheme>1</ValidationScheme>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>Username1</AttributeMask>
    <Attributes>
      <Username1>1448</Username1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>AuthenticationSecurityMethod</Entity>
    <Instance>2</Instance>
    <AttributeMask>Password</AttributeMask>
    <Attributes>
      <Password>Password2</Password>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <Attributes/>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>NumberOfParts</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part1</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part2</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part3</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part4</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part5</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part6</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part7</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part8</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part9</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part10</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part11</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part12</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part13</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part14</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>Part15</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>LargeString</Entity>
    <Instance>5</Instance>
    <AttributeMask>NumberOfParts Part1</AttributeMask>
    <Attributes>
      <NumberOfParts>1</NumberOfParts>
      <Part1>5551002</Part1>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Create ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>2</Instance>
    <Attributes>
      <SipAgentPointer>2</SipAgentPointer>
      <UserPartAor>4</UserPartAor>
      <UsernamePassword>2</UsernamePassword>
      <VoicemailServerSipUri>65535</VoicemailServerSipUri>
      <VoicemailSubscriptionExpireTime>3600</VoicemailSubscriptionExpireTime>
      <NetworkDialPlanPointer>65535</NetworkDialPlanPointer>
      <ApplicationServicesProfilePointer>65535</ApplicationServicesProfilePointer>
      <FeatureCodePointer>65535</FeatureCodePointer>
      <PptpPointer>770</PptpPointer>
    </Attributes>
  </Create>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>2</Instance>
    <AttributeMask>SipAgentPointer UserPartAor</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>2</Instance>
    <AttributeMask>SipDisplayName</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>SipUserData</Entity>
    <Instance>2</Instance>
    <AttributeMask>UsernamePassword VoicemailServerSipUri VoicemailSubscriptionExpireTime NetworkDialPlanPointer ApplicationServicesProfilePointer FeatureCodePointer PptpPointer ReleaseTimer RohTimer</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>PotsUni</Entity>
    <Instance>769</Instance>
    <AttributeMask>AdministrativeState Impedance RxGain TxGain</AttributeMask>
    <Attributes>
      <AdministrativeState>0</AdministrativeState>
      <Impedance>2</Impedance>
      <RxGain>0</RxGain>
      <TxGain>0</TxGain>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Set ar="1" ak="0">
    <Entity>PotsUni</Entity>
    <Instance>770</Instance>
    <AttributeMask>AdministrativeState Impedance RxGain TxGain</AttributeMask>
    <Attributes>
      <AdministrativeState>0</AdministrativeState>
      <Impedance>2</Impedance>
      <RxGain>0</RxGain>
      <TxGain>0</TxGain>
    </Attributes>
  </Set>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>Ont2G</Entity>
    <Instance>0</Instance>
    <AttributeMask>EqId</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>OntData</Entity>
    <Instance>0</Instance>
    <AttributeMask>MibDataSync</AttributeMask>
  </Get>
</Message>
]);


$omci->transact(q[<?xml version="1.0"?>
<Message>
  <Get ar="1" ak="0">
    <Entity>OntData</Entity>
    <Instance>0</Instance>
    <AttributeMask>MibDataSync</AttributeMask>
  </Get>
</Message>
]);

