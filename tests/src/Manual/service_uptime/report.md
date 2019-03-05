---
Test Case ID: report
Author: Aaron Klump
Created: March 4, 2019
---
## Test Scenario

The report page shows the statistics from Service Uptime with and without embedded CSS.

## Pre-Conditions

1. Make sure _Service Uptime Statistics Link_ has been entered in [the settings](/admin/reports/service-uptime/settings).

## Test Execution

1. Visit [the View Stats page](/admin/reports/service-uptime)
  - Assert you see statistics loaded from Service Uptime.
1. Visit [the settings](/admin/reports/service-uptime/settings) and remove _Service Uptime Statistics Link_
1. Click _Save configuration_. 
1. Visit [the View Stats page](/admin/reports/service-uptime)
  - Assert you see message to configure your settings.
1. Click the link in the message.
    - Assert the settings page loads.
