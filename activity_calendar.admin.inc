<?php

/**
 * @file
 * Contains logic for admin pages.
 */

/**
 * Form builder for admin form.
 */
function activity_calendar_admin_form($form, &$form_state) {
  $config = config('activity_calendar.settings');
  $path = backdrop_get_path('module', 'activity_calendar');
  // Load libraries.
  backdrop_add_css($path . '/activity_calendar.css');
  backdrop_add_css($path . '/lib/cal-heatmap.css');
  backdrop_add_js($path . '/lib/d3.min.js');
  backdrop_add_js($path . '/lib/cal-heatmap.min.js');

  $form['#config'] = 'activity_calendar.settings';
  $form['wrapper'] = [
    '#type' => 'fieldset',
    '#title' => t('Activity calendar settings'),
    '#description' => t('Use this form to customize the activity calendar settings.'),
  ];

  $form['wrapper']['calendar'] = [
    '#type' => 'fieldset',
    '#title' => t('Calendar settings'),
    '#description' => t('Configure activity calendar parameters.'),
  ];
  $form['wrapper']['calendar']['calendar_type'] = [
    '#type' => 'select',
    '#title' => t('Calendar type'),
    '#options' => array('yearly' => 'Yearly', 'monthly' => 'Monthly'),
    '#default_value' => $config->get('calendar_type'),
  ];

  // Indicate path to data file.
  $json_file = '/' . $path . '/lib/user_activity.json';

  // Change calendar type and range per user selection.
  if ($config->get('calendar_type') == 'monthly') {
    $calendar_type = 'month';
    $range = '12';
  }
  else {
    $calendar_type = 'year';
    $range = '1';
  }

  // Sync calendar with the system's first day of the week.
  if (config_get('system.date', 'first_day') == '1') {
    $first_day = "true";
  }
  else {
    $first_day = "false";
  }

  // Build activity heatmap calendar.
  $form['wrapper']['calendar']['type'] = [
    '#type' => 'item',
    '#title' => t('Activity calendar'),
    '#markup' => '
    <div id="activity-calendar">
    <div class="control-buttons">
    <button id="PreviousDomain" class="button previous">' . t('Previous') . '</button>
    <button id="NextDomain" class="button next">' . t('Next') . '</button>
    </div>
    <div id="heatmap"></div>
    </div>
    <script type="text/javascript">
    var cal = new CalHeatMap();
    cal.init({
      legendColors: ["' . $config->get('color_min') . '","' . $config->get('color_max') . '"],
    	itemSelector: "#heatmap",
    	domain: "' . $calendar_type . '",
    	subDomain: "day",
    	data: "' . $json_file . '",
      minDate: new Date(2015, 1),
      start: new Date(2021, 0),
      maxDate: new Date(2021, 0),
      onMinDomainReached: function(hit) {
      if (hit) {
          $("#PreviousDomain").attr("disabled", "disabled");
        } else {
          $("#PreviousDomain").attr("disabled", false);
        }
      },
      onMaxDomainReached: function(hit) {
      if (hit) {
          $("#NextDomain").attr("disabled", "disabled");
        } else {
          $("#NextDomain").attr("disabled", false);
        }
      },
    	cellSize: 10,
    	cellPadding: 1,
      domainGutter: 0,
    	range: ' . $range . ',
      itemName: ["' . t('point') . '", "' . t('points') . '"],
      domainDynamicDimension: false,
    	previousSelector: "#PreviousDomain",
    	nextSelector: "#NextDomain",
      legend: [' . $config->get('thresholds') . '],
      highlight: "now",
      weekStartOnMonday: ' . $first_day . ',
    });
    $("#CurrentDomain").on("click", function(event) {
      cal.rewind();
    });
    </script>',
  ];

  $form['wrapper']['calendar']['color_min'] = [
    '#type' => 'color',
    '#title' => t('Lightest color'),
    '#size' => 10,
    '#description' => t('Pick up the lightest color for highlighting the calendar cells.'),
    '#default_value' => $config->get('color_min'),
  ];
  $form['wrapper']['calendar']['color_max'] = [
    '#type' => 'color',
    '#title' => t('Darkest color'),
    '#size' => 10,
    '#description' => t('Pickup the darkest color for highlighting the calendar cells.'),
    '#default_value' => $config->get('color_max'),
  ];
  $form['wrapper']['calendar']['thresholds'] = [
    '#type' => 'textfield',
    '#title' => t('Point thresholds'),
    '#size' => 10,
    '#description' => t('Comma-seperated point thresholds that change daily activity color.'),
    '#default_value' => $config->get('thresholds'),
  ];

  $form['wrapper']['points'] = [
    '#type' => 'fieldset',
    '#title' => t('Node & Comment points'),
    '#description' => t('Select user actions which increment their activity points.'),
  ];

  $form['wrapper']['points']['nodes'] = [
    '#type' => 'checkbox',
    '#title' => t('Creating nodes'),
    '#default_value' => $config->get('nodes'),
  ];

  $form['wrapper']['points']['node_types'] = [
    '#type' => 'fieldset',
    '#title' => t('Node types'),
    '#description' => t('Select content types that are counted when collecting activity points.'),
    '#states' => [
      'invisible' => [
        ':input[name="nodes"]' => ['checked' => FALSE],
      ],
    ],
  ];
  foreach (node_type_get_names() as $type => $name) {
    $form['wrapper']['points']['node_types']['node_type_' . $type] = [
      '#type' => 'checkbox',
      '#title' => t(check_plain($name)),
      '#default_value' => ($config->get('node_type_' . $type)) ? TRUE : FALSE,
    ];
    $form['wrapper']['points']['node_types'][$type . '_points'] = [
      '#type' => 'textfield',
      '#title' => t(check_plain($name) . ' ' . 'points'),
      '#description' => t('Indicate how many points user gets by submitting a new ' . $type . '.'),
      '#default_value' => ($config->get($type . '_points')) ? $config->get($type . '_points') : $config->get('node_points'),
      '#size' => 10,
      '#states' => [
        'invisible' => [
          ":input[name=node_type_$type]" => ['checked' => FALSE],
        ],
      ],
    ];
  }

  if (module_exists('comment')) {
    $form['wrapper']['points']['comments'] = [
      '#type' => 'checkbox',
      '#title' => t('Submitting comments'),
      '#default_value' => $config->get('comments'),
    ];
    $form['wrapper']['points']['comment_node_types'] = [
      '#type' => 'fieldset',
      '#title' => t('Node type comments'),
      '#description' => t('Select content types that are counted when collecting activity points for their comments.'),
      '#states' => [
        'invisible' => [
          ':input[name="comments"]' => ['checked' => FALSE],
        ],
      ],
    ];
    foreach (node_type_get_names() as $type => $name) {
      $type_comments = $type . '_comments';
      $form['wrapper']['points']['comment_node_types'][$type_comments] = [
        '#type' => 'checkbox',
        '#title' => t(check_plain($name)),
        '#default_value' => ($config->get($type_comments)) ? TRUE : FALSE,
      ];
      $form['wrapper']['points']['comment_node_types'][$type . '_comment_points'] = [
        '#type' => 'textfield',
        '#title' => t(check_plain($name) . ' ' . 'comment points'),
        '#description' => t('Points for submitting a ' . $type . ' comment.'),
        '#default_value' => ($config->get($type . '_comment_points')) ? $config->get($type . '_comment_points') : $config->get('comment_points'),
        '#size' => 10,
        '#states' => [
          'invisible' => [
            ":input[name=$type_comments]" => ['checked' => FALSE],
          ],
        ],
      ];
    }
  }

  // Create fieldset wrapper for contributed modules.
  $form['wrapper']['contrib'] = [
    '#type' => 'fieldset',
    '#title' => t('Contributed module points'),
    '#description' => t('Configure activity points for contributed modules.'),
  ];

  // Check if any contributed module implemented hook_calendar().
  module_invoke_all('calendar');
  foreach (module_implements('calendar') as $module) {
    $result[$module] = module_invoke($module, 'calendar');
    $form['wrapper']['contrib'][$module] = [
      '#type' => 'textfield',
      '#title' => t($result[$module]['module_name']),
      '#description' => t('Number of points assigned for this activity.'),
      '#size' => 10,
      '#default_value' => $config->get($module),
    ];
  }
  // Unset fieldset wrapper for contributed modules if none found.
  if (!isset($module)) {
    unset($form['wrapper']['contrib']);
  }

  return system_settings_form($form);
}

/**
 * Form validate for admin form.
 */
function activity_calendar_admin_form_validate($form, &$form_state) {
  if ($form_state['values']['nodes'] == 0 && $form_state['values']['comments'] == 0) {
    form_set_error('activity_calendar', t('You must select at least one of user actions under the "Node & Comment points" section.'));
  }
  if ($form_state['values']['nodes'] == 1) {
    foreach (node_type_get_names() as $type => $name) {
      if ($form_state['values']['node_type_' . $type] == 1) {
        $enabled_content_type = TRUE;
        break; // Found at least one enabled content type.
      }
    }
    if (!isset($enabled_content_type)) {
      form_set_error('activity_calendar', t('You must select at least one of content types under the "Node types" section.'));
    }
  }
  if ($form_state['values']['comments'] == 1) {
    foreach (node_type_get_names() as $type => $name) {
      if ($form_state['values'][$type . '_comments'] == 1) {
        $enabled_ct_comments = TRUE;
        break; // Found at least one enabled content type.
      }
    }
    if (!isset($enabled_ct_comments)) {
      form_set_error('activity_calendar', t('You must select at least one of content types under the "Node type comments" section.'));
    }
  }
}
