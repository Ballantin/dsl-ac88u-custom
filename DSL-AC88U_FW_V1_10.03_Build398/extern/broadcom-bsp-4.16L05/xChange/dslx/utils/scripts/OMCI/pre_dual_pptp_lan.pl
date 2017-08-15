#!/usr/bin/perl -w

use strict;
use warnings;

## Automates and asks for network addresses for SIP ##

my $line;
my $sip_network;
my $sip_nethex;
my $gateway;
my $gate_hex;
my $filepath = '/usr/local/etc/opensips/opensipsctlrc';

## if opensipctlrc exists, extract sip info ##
if(-e $filepath) 
{
   open( FILE, $filepath) ;
   while ($line = <FILE>) 
   {
   
      if ($line =~ /SIP_DOMAIN=/)
      {
         my $net_length = length ($line);
		 my $temp_length = length ("SIP_DOMAIN=");
		 $net_length = $net_length - $temp_length - 1;
         $sip_network = substr($line, $temp_length, $net_length);
         my @IP = split(/\./, $sip_network);
         my @temp = @IP;
		
         for (my $i= 0; $i <4; $i++) 
         {
            $temp[$i] = sprintf("%x",$temp[$i]);
            if (length($temp[$i]) == 1 ) 
            {
               $temp[$i] = "0$temp[$i]";
            }
         }
		
         $sip_nethex = "$temp[0]$temp[1]$temp[2]$temp[3]";
         $IP[3] = 1;
         $gateway = "$IP[0].$IP[1].$IP[2].$IP[3]";		
         @IP = split(/\./, $gateway);	
         $gate_hex = "$temp[0]$temp[1]$temp[2]01";
      }
	  
   }
   close(FILE);
}
## Use user's input ##
else 
{
   ## Manual insert since file cannot be found ##
   print "File opensipsctlrc not found. Please manually enter the SIP networks\n";
   print "Enter the SIP network (xxx.xxx.xxx.xxx)\n";
   $sip_network = <>;
   
   my @IP = split(/\./, $sip_network);
   my @temp = @IP;
   
   for (my $i= 0; $i <4; $i++) 
   {
      $temp[$i] = sprintf("%x",$temp[$i]);
      if (length($temp[$i]) == 1 ) 
      {
         $temp[$i] = "0$temp[$i]";
      }
   }

   $sip_nethex = "$temp[0]$temp[1]$temp[2]$temp[3]";
   $IP[3] = 1;
   $gateway = "$IP[0].$IP[1].$IP[2].$IP[3]";		
   @IP = split(/\./, $gateway);	
   $gate_hex = "$temp[0]$temp[1]$temp[2]01";
   @IP = split (/\./,$sip_network);
   $sip_network = "$IP[0].$IP[1].$IP[2].$IP[3]";
   $sip_network = substr ($sip_network, 0, length($sip_network)-1);

}

print "OpenSIPS address: $sip_network\n";
print "OpenSIPS hex: $sip_nethex\n";
print "Gateway Address: $gateway\n";
print "Gateway Hex: $gate_hex\n";
    
my $filename = 'dual_pptp_lan.pl';
my $find = 'SIPNET';
my $find2 = 'SIPHEX';
my $find3 = 'GATEHEX';

my $replace = "$sip_network";
my $replace2 = "$sip_nethex";
my $replace3 = "$gate_hex";
   
open ( FILE, "<$filename");
my @lines = <FILE>;
close FILE;

open (STDOUT, ">$filename");

for (@lines) 
{
   if ( s/$find/$replace/ig ) 
   {
      print;
   }
   elsif (s/$find2/$replace2/ig) 
   {
	  print;
   }
   elsif (s/$find3/$replace3/ig) {
   
      print;
   }
   else 
   {
      print;
   }
}
close STDOUT;   
   
#system("./dual_pptp_lan.pl -tar $gateway");

open (STDOUT, ">$filename");

for (@lines) 
{
   if ( s/$replace/$find/ig ) 
   {
      print;
   }
   elsif (s/$replace2/$find2/ig) 
   {
	  print;
   }
   elsif (s/$replace3/$find3/ig) 
   {
      print;
   }
   else 
   {
      print;
   }
}
close STDOUT;  

