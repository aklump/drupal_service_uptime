<?php /**
 * @file
 * Contains \Drupal\service_uptime\Controller\DefaultController.
 */

namespace Drupal\service_uptime\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Url;
use Drupal\service_uptime\Service\ServiceUptime;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException;

/**
 * Default controller for the service_uptime module.
 */
class DefaultController extends ControllerBase {

  public function __construct(ServiceUptime $service_uptime) {
    $this->serviceUptime = $service_uptime;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('service_uptime')
    );
  }

  public function statisticsPage() {
    $build = [];

    if (!($qs = $this->serviceUptime->getAccountQueryString())) {
      $build[] = [
        '#markup' => t(ServiceUptime::NOT_CONFIGURED_MESSAGE, [
          ':admin_settings_url' => Url::fromRoute('service_uptime.admin_settings')
            ->toString(),
        ]),
      ];

    }
    else {
      try {
        if (!($output = $this->serviceUptime->getStatisticsMarkup())) {
          $build[] = ['#markup' => t('Error processing request.')];
        }
        $build[] = ['#markup' => $output];
      }

      catch (\Exception $exception) {
        $build[] = [
          '#markup' => t('Your host provider has disabled all functions to handle remote pages. You may still view your statistics by logging in to your account at <a href=":url">Service Uptime</a>.', [
            ':url' => $this->serviceUptime->getUrl(),
          ]),
        ];
      }

      $build[] = [
        '#prefix' => '<section class="service-uptime-stats__drupal-footer">',
        '#suffix' => '</section>',
        '#markup' => $this->t('<a href=":url" target="_blank">Goto your account at Service Uptime.</a> &rarr;', [
          ':url' => $this->serviceUptime->getUrl('/users/services.php'),
        ]),
      ];
    }

    $build = [
      '#prefix' => '<div class="service-uptime-stats">',
      '#suffix' => '</div>',
      0 => $build,
    ];

    return $build;
  }

  /**
   * Render the page for the service uptime check.
   *
   * This page will take $_GET['s'] as the seed and return a public key based on
   * that seed However if that return doesn't equal the private key then you get
   * a 404.
   */
  public function checkPage(Request $request) {
    $fail = $this->config('service_uptime.settings')
      ->get('fail_on_next');

    // This is used to force a failure; it is set in the admin settings page.
    if ($this->config('service_uptime.settings')
      ->get('fail_on_next')) {
      $this->configFactory->getEditable('service_uptime.settings')
        ->clear('fail_on_next')
        ->save();

      throw new AccessDeniedHttpException();
    }
    $seed = $request->get('s');
    if (empty($seed) || $this->serviceUptime->getPublicHash($seed) != $this->serviceUptime->getPublicHash()) {
      throw new AccessDeniedHttpException();
    }

    return new Response(implode('', [
      $this->serviceUptime->getPublicHash(time()),
      $this->serviceUptime->getPublicHash($seed),
      $this->serviceUptime->getPublicHash(time() / 2),
    ]), 200, ['Content-Type' => 'text/plain']);
  }

}
