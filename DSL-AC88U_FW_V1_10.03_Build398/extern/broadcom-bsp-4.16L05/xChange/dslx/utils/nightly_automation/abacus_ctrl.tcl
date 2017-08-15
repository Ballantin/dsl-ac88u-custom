package require Itcl
package require tdom
package require tclreadyapi

# Abacus5000 System controller IP
variable SC_IP 		"10.1.0.32"

# Create application singleton
::aba::ApiApplication app

# Create system information singleton
::aba::ApiSystemInformation sysinfo

# Create test singleton
::aba::ApiTest test

# Create results singleton
::aba::ApiResults results

# Create test duration singleton
::aba::DatTestduration duration

# Create report singleton
::aba::ApiReports reports

# Create report setup singleton
::aba::ApiRGEReportAdvancedSetup reportsetup

# Create Partition and Timing singleton
::aba::ApiPnT pnt

# procedure to load and test
proc runtest { env duraHr duraMin report_template phonebook dirname} {
	puts "Connection set up!\nLoading environment $env..."
	set loadvar [app Load $env]
	set loadvar [string compare $loadvar "OK"]
	if { $loadvar != 0 } {
		puts "Unable to load ::[app GetLastError]"
	} else {
		puts "$env Loaded!!\nStarting Test..."
		
		# Setting up test duration
		duration.type SetValue "Interval"
		duration.minutes SetValue $duraMin
		duration.hours SetValue $duraHr
		set paramvalue [duration ExportToXMLString]
		set result [test SetDuration $paramvalue]
		if { 1 != $result } {
			puts "Unable to set duration::[test GetLastError]"
			exit 1
		}
		puts "Test duration is set to $duraHr hours $duraMin minutes"

		set pntresult [pnt SetAddressBook $phonebook]
		if { 1 != $pntresult } then {
			puts "Unable to set the address book! :: [pnt GetLastError]"
		} else {
			puts "The address book is successfully set up"
		}

		# Start the test
		set teststart [test Start]
		set testresult [string compare $teststart "OK"]
		if { 0 != $testresult } {
			puts "Unable to start ::[test GetLastError]"
			exit 1
		}
		puts "Test started succesfully"

		while {1} {
			# Check test status
			set teststatus [test GetStatus]
			set ret [string compare $teststatus "<env-status>DONE</env-status>\n"]

			# Test ended
			if { 0 == $ret } {
				# Generate report
				reports SetRGEConfig $report_template
				set ret [reports Generate]
				if { ![string equal $ret "OK"] } {	
					puts "Unable to generate the test report ::[reports GetLastError]"
					exit 1
				}
				puts "Report is successfully generated"

				# Get the date and contruct report folder name
				set ret [reports Download $dirname]
				if { ![string equal $ret "OK"] } {
					puts "Unable to download the test report ::[reports GetLastError]"
					exit 1
				}
				puts "Report is successfully downloaded!"

				# Get Event Count
				set eventcount [results GetEventsCount]
				puts "Number of events: $eventcount"

				app Exit
				if { $eventcount > 2 } {
					# report error
					exit 2
				}
				exit 0
			}
			::aba::sleep 60
		}
	}
}

####################################################
# All singletons are created, start Abacus session
####################################################
# Command line arguments
if { $argc != 6 } {
	puts "USAGE: abacus_ctrl.tcl <AbacusEnv> <DurationHR> <DurationMIN> <ReportTemplate> <PhonebookTemplate> <reportDirname>"
	exit 1
} else {
	set Abacus_Env [lindex $argv 0]
	set DurationHR [lindex $argv 1]
	set DurationMIN [lindex $argv 2]
	set ReportTemplate [lindex $argv 3]
	set PhonebookTemplate [lindex $argv 4]
	set ReportDirname [lindex $argv 5]
}

# Connect to Abacus automation server
app Enter $::aba::AUTOMATION_SERVER_ADDRESS
puts "Abacus Automation Server connected."

set isready [app IsAbacusReady]
if { $isready != 1 } {
	puts [app GetLastError]
} else {
	puts "Setting up the connection to $SC_IP"

	set sc [sysinfo SetConnection $SC_IP ""]
	if { 1 != $sc } {
		puts "Unable to Connect::[sysinfo GetLastError]"
		exit 1
	} else {
		source $ReportTemplate
		source $PhonebookTemplate
		# Run test continously until it terminates
		runtest $Abacus_Env $DurationHR $DurationMIN $reportXml $phonebook $ReportDirname
	}
}
