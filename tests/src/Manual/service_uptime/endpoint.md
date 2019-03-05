---
Test Case ID: endpoint
Author: Aaron Klump
Created: March 4, 2019
---
## Test Scenario

The endpoint page loads with embedded token string.

## Pre-Conditions

1. Make sure _Service Uptime Statistics Link_ has been entered in [the settings](/admin/reports/service-uptime/settings).

## Test Execution

1. Visit [the Settings](/admin/reports/service-uptime/settings).
1. Click on _Monitor Settings_ to reveal the hidden data.
    - Assert _Name_ is the site name.
    - Assert _Service_ is `web_page`
    - Assert _Page Url_ is not empty.
    - Assert _Search string_ is not empty.
1. Click the _Test_ link next to the _Page Url_.
  - Assert a new window opens.
  - Assert you see a string of text on a white page.
1. Return to [the Settings](/admin/reports/service-uptime/settings).
1. Copy the value for _Search string_
  - Assert that string exists on the white page of text.
1. Return to [the Settings](/admin/reports/service-uptime/settings).
1. Click _Advanced_ to expose more settings.
1. Place a check in _Force a failure on next check_.
1. Click _Save configuration_.
1. Click the _Test_ link next to the _Page Url_.
  - Assert the page opens with _Not Found_
1. Reload the page.
   - Assert you see a string of text on a white page. 
