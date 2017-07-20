SUMMARY --

The ExactTarget API module provides abstraction of the ExactTarget API for
use by other modules. By itself, this module provides no functionality and 
should only be installed if another module requires it as a dependency.

Support is included the for FuelSDK-PHP library v1.0.0 or later ( https://github.com/salesforce-marketingcloud/FuelSDK-PHP )
and the Libraries API module. Documentation for the FuelSDK-PHP library can be found at https://developer.salesforce.com/docs/atlas.en-us.mc-sdks.meta/mc-sdks/using-a-client-library.htm


For a full description of the module, visit the project page:
  http://drupal.org/project/exact_target_api

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/exact_target_api


-- REQUIREMENTS --

Existing ExactTarget account with API access.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure integration with ExactTarget API in Administration > Configuration >
  System > ExactTarget API, settings can be found in your App at
  https://appcenter.s1.marketingcloudapps.com:

  - Select the "Authentication Target" for the API calls. Possible options are:
      - S1 Instance
      - S4 Instance
      - S6 Instance
      - S7 Instance
      - Production Support (test) Instance


  - ExactTarget API Client ID
  - ExactTarget API Client Secret

-- CUSTOMIZATION --

Under the hood the FuelSDK uses cURL to make requests to the appropriate API
endpoint. As we are currently using the SDK unmodified any cURL customizations
need to be set in Environment Variables
  - Proxy should be able to be set using
    putenv('http_proxy=http://10.0.5.5:8080');
    in your settings.php file, as proxy usage something I need for my use case I
    will be testing this and making accomodations if necessary.

-- TO DO --
* update and test all api functions


-- CONTACT --

Current maintainers:
* Katrin Silvius (nonsie) - http://drupal.org/user/29899
* Robert Bates (arpieb) - http://drupal.org/user/533416
* James Glasgow (jrglasgow) - (7|8).x-2.x branches - https://www.drupal.org/user/36590

This project has been sponsored by:
* Cool Blue Interactive
  Visit http://www.coolbluei.com/ for more information.

* Live Axle Interactive
  We are makers of interactive digital experiences.
  Visit http://www.liveaxle.com for more information.
