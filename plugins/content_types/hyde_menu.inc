<?php

$plugin = array(
  'title' => t('Hyde Menu'),
  'category' => t('Hyde'),
  'no title override' => TRUE,
  'defaults' => array('admin_title' => '', 'title' => '', 'menu_name' => 'main-menu', 'menu_style' => ''),
);


// TODO: much better handling of menus, item classes, active item
// TODO: Hierarchical menus.
function hyde_hyde_menu_content_type_render($subtype, $conf, $panel_args) {
  $theme = hyde_get_theme();
  // dpm(get_defined_vars());
  if (!empty($conf['menu_style']) && isset($theme->menu_styles[$conf['menu_style']])) {
    $style = $theme->menu_styles[$conf['menu_style']];
    $menu = menu_navigation_links($conf['menu_name']);
    $block->content = theme('links', array(
      'links' => $menu,
      'attributes' => array('class' => array($style['wrapper'])),
    ));
  }
  return $block;
}


function hyde_hyde_menu_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // Add style options
  $theme = hyde_get_theme();
  $styles = array();
  if (!empty($theme->menu_styles)) {
    foreach ($theme->menu_styles as $style) {
      $styles[$style['name']] = $style['label'];
    }
  }
  $form['menu_style'] = array(
    '#type' => 'select',
    '#title' => t('Menu style'),
    '#options' => $styles,
    '#default_value' => $conf['menu_style'],
  );
  // Add menu options
  $form['menu_name'] = array(
    '#type' => 'select',
    '#title' => t('Menu name'),
    '#options' => menu_get_menus(),
    '#default_value' => $conf['menu_name'],
  );

  return $form;
}

function hyde_hyde_menu_content_type_edit_form_submit($form, &$form_state) {
  foreach (array('menu_name', 'menu_style') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
