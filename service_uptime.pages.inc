<?php
/**
 * @file
 * Handles the extra page functions, loaded as needed.
 */

/**
 * Generate the statistics page.
 */
function service_uptime_stats_page() {
  $build = [];

  if (!($qs = service_uptime_get_account_query_string())) {
    $build[] = [
      '#markup' => t(SERVICE_UPTIME_NOT_CONFIGURED_MESSAGE, ['!admin_settings_url' => url(SERVICE_UPTIME_PATH_DRUPAL_SETTINGS)]),
    ];
  }
  else {
    try {
      if (!($output = service_uptime_get_statistics_markup())) {
        $build[] = ['#markup' => t('Error processing request.')];
      }
      $build[] = ['#markup' => $output];
    }
    catch (\Exception $exception) {
      $build[] = [
        '#markup' => t('Your host provider has disabled all functions to handle remote pages. You may still view your statistics by logging in to your account at <a href="!url">Service Uptime</a>.', array(
          '!url' => service_uptime_url(),
        )),
      ];
    }

    $build[] = [
      '#markup' => '<div class="footer">' . t('For more details log in to your account at <a href="!url">Service Uptime</a>', array('!url' => service_uptime_url('/users/services.php'))) . '</div><!-- /.footer -->',
    ];
  }

  return $build;
}

/**
 * Render the page for the service uptime check.
 *
 * This page will take $_GET['s'] as the seed and return a public key based on
 * that seed However if that return doesn't equal the private key then you get
 * a 404.
 */
function service_uptime_check_page() {
  // This is used to force a failure; it is set in the admin settings page.
  if (variable_get('service_uptime_force_failure', NULL)) {
    variable_del('service_uptime_force_failure');

    return MENU_NOT_FOUND;
  }
  $seed = isset($_GET['s']) ? $_GET['s'] : NULL;
  if (empty($seed) || service_uptime_get_public_hash($seed) != service_uptime_get_public_hash()) {
    return MENU_NOT_FOUND;
  }
  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  echo(implode('', [
    service_uptime_get_public_hash(time()),
    service_uptime_get_public_hash($seed),
    service_uptime_get_public_hash(time() / 2),
  ]));
  drupal_exit();
}
