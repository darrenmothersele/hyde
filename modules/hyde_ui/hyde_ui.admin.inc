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
