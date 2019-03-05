<?php

/**
 * @file
 * Contains \Drupal\service_uptime\Form\ServiceUptimeAdminSettings.
 */

namespace Drupal\service_uptime\Form;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Url;
use Drupal\service_uptime\Service\ServiceUptime;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provide the admin settings form.
 */
class ServiceUptimeAdminSettings extends ConfigFormBase {

  /**
   * An instance of our core service.
   *
   * @var \Drupal\service_uptime\ServiceUptime
   */
  protected $serviceUptime;

  /**
   * Constructs a \Drupal\system\ConfigFormBase object.
   *
   * @param \Drupal\service_uptime\Service\ServiceUptime $service_uptime
   *   An instance of our core service.
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   */
  public function __construct(
    ServiceUptime $service_uptime,
    ConfigFactoryInterface $config_factory
  ) {
    parent::__construct($config_factory);
    $this->serviceUptime = $service_uptime;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('service_uptime'),
      $container->get('config.factory')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'service_uptime_admin_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('service_uptime.settings');
    $config->set('fail_on_next', $form_state->getValue('fail_on_next'));
    $config->set('stats_url', $form_state->getValue('stats_url'));
    $config->save();
    parent::submitForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['service_uptime.settings'];
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $form = ['#tree' => FALSE];
    $form['#attached']['library'][] = 'service_uptime/service_uptime';
    $stats_link = $this->config('service_uptime.settings')
      ->get('stats_url');

    if (!$stats_link) {
      $form['signup'] = [
        '#markup' => $this->t('<h2><a href=":url" target="_blank">Get your free account &rarr;</a></h2>', [
          ':url' => $this->serviceUptime->getUrl(),
        ]),
      ];
    }

    if ($hash = $this->serviceUptime->getPublicHash()) {
      $form['edit_service'] = [
        '#type' => 'details',
        '#title' => $this->t('Monitor Settings'),
        '#description' => $this->t('Use these values when adding your "website" monitor to your Service Uptime account.'),
        '#weight' => -10,
        '#open' => empty($stats_link),
      ];

      $form['edit_service']['name'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Name'),
        '#value' => $this->config('system.site')->get('name'),
        '#attributes' => ['readonly' => TRUE],
      ];

      $form['edit_service']['service'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Service'),
        '#value' => 'web page',
        '#attributes' => [
          'readonly' => TRUE,
        ],
      ];

      // TODO Do we want to/can we use a CSRF token? https://www.drupal.org/docs/8/api/routing-system/access-checking-on-routes instead?
      $url = Url::fromRoute('service_uptime.check_page', [
        's' => $this->config('service_uptime.settings')
          ->get('public_key'),
      ], [
        'absolute' => TRUE,
      ]);
      $form['edit_service']['page_url'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Page Url'),
        '#value' => $url->toString(),
        '#attributes' => ['readonly' => TRUE],
        '#field_suffix' => $this->l($this->t('Test &rarr;'), $url, [
          'html' => TRUE,
          'attributes' => ['target' => '_blank'],
        ]),
      ];


      $form['edit_service']['service_uptime_public_key'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Search string'),
        '#description' => $this->t('Page should contain this text'),
        '#default_value' => $hash,
        '#size' => 60,
        '#attributes' => [
          'readonly' => TRUE,
        ],
      ];
    }

    $form['stats_url'] = [
      '#type' => 'textfield',
      '#description' => $this->t('<p>Click <a href=":url" target="_blank">here</a> and locate the "Direct Link" URL; copy it in this box.  It will resemble <strong>:example_url</strong></p><p>You also will have to <a href=":public" target="_blank">enable</a> public statistics.</p>', [
        ':url' => $this->serviceUptime->getUrl('/users/openstat.php'),
        ':example_url' => ServiceUptime::EXAMPLE_LINK,
        ':public' => $this->serviceUptime->getUrl('/users/openstatmulti.php'),
      ]),
      '#title' => $this->t('Service Uptime Statistics Link'),
      '#default_value' => $stats_link,
      '#required' => FALSE,
      '#size' => 120,
    ];

    $form['dev'] = [
      '#type' => 'details',
      '#title' => $this->t('Advanced'),
      '#open' => FALSE,
    ];

    $form['dev']['fail_on_next'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Force a failure on next check'),
      '#description' => $this->t('(Use sparingly as it will skew your results; do this to check if the failure email is working correctly.)'),
      '#default_value' => $this->config('service_uptime.settings')
        ->get('fail_on_next'),
    ];

    return parent::buildForm($form, $form_state);
  }

  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    if ($form_state->getValue(['stats_url']) && !preg_match(ServiceUptime::REGEX, $form_state->getValue(['stats_url']))) {
      $form_state->setErrorByName('stats_url', $this->t('The format of your link is incorrect.  It should look like this: <strong>:example_url</strong>', [
        ':example_url' => ServiceUptime::EXAMPLE_LINK,
      ]));
    }
  }

}
