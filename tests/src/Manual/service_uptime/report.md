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

1. Visit [the settings](/admin/reports/service-uptime/settings) and uncheck _Embed the Service Uptime stylesheet on your Drupal statistics page?_
1. Click _Save configuration_. 
1. Visit [the View Stats page](/admin/reports/service-uptime)
  - Assert you see statistics loaded from Service Uptime.
1. Visit [the settings](/admin/reports/service-uptime/settings) and check _Embed the Service Uptime stylesheet on your Drupal statistics page?_
1. Click _Save configuration_. 
1. Visit [the View Stats page](/admin/reports/service-uptime)
  - Assert you see statistics loaded from Service Uptime.
  - Assert the style is now influenced by the embedded styles from _Service Uptime_.
1. Visit [the settings](/admin/reports/service-uptime/settings) and remove _Service Uptime Statistics Link_
1. Click _Save configuration_. 
1. Visit [the View Stats page](/admin/reports/service-uptime)
  - Assert you see message to configure your settings.
