<?php
// $Id$

/**
 * @file
 * Handles the extra page functions, loaded as needed
 *
 * @ingroup service_uptime
 * @{ 
 */

/**
 * Generate the statistics page
 */
function service_uptime_stats_page() {
  if (variable_get('service_uptime_stock_css', TRUE)) {
    drupal_add_css(drupal_get_path('module', 'service_uptime') .'/css/service_uptime_stock.css');  
  }
  
  $output = '';

  if (!($qs = _service_uptime_get_qs())) {
    $output .= theme('service_uptime_configure_message');
  }
  else {
    if(intval(get_cfg_var('allow_url_fopen')) && function_exists('file_get_contents')) {
      if(!($content = @file_get_contents(SERVICE_UPTIME_URL_STATS . $qs))) {
          $output .= t('Error processing request');
      }
      else {
          $output .= $content;
      }
    }
    elseif(intval(get_cfg_var('allow_url_fopen')) && function_exists('file')) {
      if(!($content = @file(SERVICE_UPTIME_URL_STATS . $qs))) {
          $output .= t('Error processing request');
      }
      else {
          $output .= @join('', $content);
      }
    }
    elseif(function_exists('curl_init')) {
      $ch = curl_init (SERVICE_UPTIME_URL_STATS . $qs);
      curl_setopt ($ch, CURLOPT_HEADER, 0);
      curl_exec ($ch);
    
      if(curl_error($ch))
          $output .= t('Error processing request');
    
      curl_close ($ch);
    }
    else {
      $output .= t('Your host provider has disabled all functions to handle remote pages. You may still view your statistics by logging in to your account at @link', array(
        '@link' => _service_uptime_l(),
      ));
    }
    //remove the footer
    $output = preg_replace('/(<TR>.*<\/TR>\s*)(<\/TABLE>)/i', '\2', $output);
    
    $output .= '<div class="footer">' . t('For more details log in to your account at !link', array(
      '!link' => _service_uptime_l(NULL, 'https://www.serviceuptime.com/users/services.php'),
    )) . '</div><!-- /.footer -->' . "\n";    
  }

  return $output;
}

/**
 * Formats the message to users that their site needs to be configured.
 *
 * @ingroup themeable
 */
function theme_service_uptime_configure_message() {
  $output = '';
  $output .= t('You have not yet configured your Service Uptime settings; please <a href="@url">click here</a>.', array(
    '@url' => url(SERVICE_UPTIME_PATH_DRUPAL_SETTINGS),
  )) . "\n";
  return $output;
}

/**
 * Render the page for the service uptime check
 *
 * This page will take $_GET['s'] as the seed and return a public key based on that seed
 * However if that return doesn't equal the private key then you get a 404
 *
 */
function service_uptime_check_page() {
  $output = '';
  $seed = isset($_GET['s']) ? $_GET['s'] : NULL;
  
  if (empty($seed) || _service_uptime_hash($seed) != _service_uptime_hash()) {
    return drupal_not_found();
  }
  
  //this is used to force a failure; it is set in the admin settings page
  elseif (variable_get('service_uptime_force_failure', NULL)) {
    variable_del('service_uptime_force_failure');
    return drupal_not_found();
  }
  
  //this is just a way to obfuscate a bit more; adding time stuff
  $output .= _service_uptime_hash(time()) . _service_uptime_hash($seed) . _service_uptime_hash(time() / 2);
  
  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  exit($output);
}

/** @} */ //end of group service_uptime
