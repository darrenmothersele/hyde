<?php




function hyde_ui_sync_form($form, &$form_state) {
  $form = array();

  $debug = array();
  ctools_include('plugins');

  if (function_exists('kprint_r')) {
    $form['debug'] = array(
      '#markup' => kprint_r(hyde_get_theme(), TRUE),
    );
  }
  $form['flush_all'] = array(
    '#title' => t('Flush all caches'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );

  $form['sync'] = array(
    '#type' => 'submit',
    '#value' => t('Sync'),
  );
  return $form;
}

function hyde_ui_sync_form_submit($form, &$form_state) {
  $batch = array(
    'operations' => array(array('hyde_build_theme', array())),
    'finished' => 'hyde_ui_sync_batch_finished',
  );
  if ($form_state['values']['flush_all']) {
    $batch['operations'][] = array('drupal_flush_all_caches', array());
  }
  batch_set($batch);
}


function hyde_ui_admin_page() {
  $content = array();

  $content['sync_form'] = drupal_get_form('hyde_ui_sync_form');

  return $content;
}


function hyde_ui_atom_generator_form($form, &$form_state) {


  $groups = array();
  $options = array();

  foreach (hyde_atom_fetch_all() as $key => $item) {
    if (!isset($item['group'])) {
      $item['group'] = t('Common');
    }
    else {
      $item['group'] = t('@group', array('@group' => $item['group']));
    }
    $item['title'] = t('@title', array('@title' => $item['title']));
    $groups[$item['group']][$key] = $item;
    $options[$item['group']][$key] = $item['title'];
  }
  ksort($groups);

  //dpm($groups);

  $form['hyde_atoms'] = array(
    '#type' => 'select',
    '#title' => 'Atom generator',
    '#options' => $options,
  );

  $form['generate'] = array(
    '#type' => 'button',
    '#value' => t('Generate'),
    '#ajax' => array(
      'callback' => 'hyde_atom_generator_js',
      'wrapper' => 'hyde-atom-wrapper',
      'method' => 'replace',
    ),
  );

  $form['example'] = array(
    '#prefix' => '<div id="hyde-atom-wrapper">',
    '#suffix' => '</div>',
  );
  $form['example']['inner'] = array(
    '#markup' => t('Select an atom to generate code'),
  );

  return $form;
}

function hyde_atom_fetch_all() {
  $path = drupal_get_path('module', 'styleguide');
  // Include modules for which we implement styleguides.
  $modules = module_list();
  foreach ($modules as $module) {
    if (file_exists($path . '/modules/' . $module . '.inc')) {
      include_once $path . '/modules/' . $module . '.inc';
    }
  }

  // Get visual testing elements.
  $items = module_invoke_all('styleguide');
  drupal_alter('styleguide', $items);
  return $items;
}

function hyde_atom_fetch($atom_name) {
  $items = hyde_atom_fetch_all();
  //dpm($items[$atom_name]);
  return $items[$atom_name];
}

function hyde_atom_generator_js($form, &$form_state) {
  //dpm($form_state['values']);
  unset($form['example']['inner']);
  $element = hyde_atom_fetch($form_state['values']['hyde_atoms']);
  $atom = theme('styleguide_item', $element);
  if (is_array($element['content'])) {
    $element['content'] = drupal_render($element['content']);
  }

  $form['example']['inner']['code'] = array(
    //'#type' => 'textarea',
    '#prefix' => '<div style="border:1px solid #ccc; padding: 1em; margin-bottom: 1em;"><textarea style="width:100%;height:200px">',
    '#suffix' => '</textarea></div>',
    '#markup' => ($element['content']),
  );
  $form['example']['inner']['example'] = array(
    '#prefix' => '<div style="border:1px solid #ccc; padding: 1em; margin-bottom: 1em;">',
    '#suffix' => '</div>',
    '#markup' => $element['content'],
  );
  return $form['example'];
}


function hyde_ui_admin_settings_form($form, &$form_state) {
  $form = array();

  $form['hyde_prototype_path'] = array(
    '#title' => t('Prototype path'),
    '#description' => t('Full file system path to the HTML prototype.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('hyde_prototype_path', ''),
  );

  return system_settings_form($form);
}
