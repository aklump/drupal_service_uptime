---
Test Case ID: status
Author: Aaron Klump
Created: March 4, 2019
---
## Test Scenario

The status page reflects the state of the settings.

## Pre-Conditions

1. Make sure _Service Uptime Statistics Link_ has been entered in [the settings](/admin/reports/service-uptime/settings).

## Test Execution

1. Visit [the Settings](/admin/reports/service-uptime/settings).
1. Cut the value of _Service Uptime Statistics Link_; save for later.
1. Click _Save configuration_.
  - Assert _Monitor Settings_ is open, exposing it's fields.
  - Assert the link _Get your free account_ is present.
1. Click _Get your free account_
  - Assert a new window opens to Service Uptime website.  
1. Visit [the status page](/admin/reports/status)
  - Assert you see a warning indicating service uptime is not yet configured.
1. Click on the link in the warning
  - Assert you are taken to the settings page.  
1. Paste in the _Service Uptime Statistics Link_
1. Click _Save configuration_.
  - Assert _Monitor Settings_ is closed.
  - Assert the link _Get your free account_ is not showing.
1. Visit [the status page](/admin/reports/status)
  - Assert Service Uptime indicates connected and ready.
1. Click on the view stats link in the status message.
  - Assert your stats page loads. 
