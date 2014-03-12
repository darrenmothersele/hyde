<?php

$plugin = array(
  'title' => t('Hyde Base Layout'),
  'category' => t('Hyde'),
  'icon' => 'layout.png',

  'theme' => 'hyde_panels_layout',
  'admin theme' => 'hyde_panels_admin_layout',

  'get child' => 'hyde_panels_get_sublayout',
  'get children' => 'hyde_panels_get_sublayouts',

);

function theme_hyde_panels_layout($vars) {
  // dpm(func_get_args());
  $layout = $vars['layout'];
  $template = $layout['layout']['template'];
  return _hacked_hyde_generic_theme_function($template, $vars);
}

function theme_hyde_panels_admin_layout($vars) {
  // dpm(func_get_args());
  $output = '';
  foreach ($vars['content'] as $name => $content) {
    $output .= $content;
  }
  return $output;
}


function _hacked_hyde_generic_theme_function($template, &$vars) {
  //hyde_dpm($vars, 'th');
  // dpm($vars); return;
  extract($vars, EXTR_SKIP);
  ob_start();
  //dpm($theme);
  //dpm($theme->{$key}[$hook]['template']);
  eval('?>' . $template . '<?php ');
  return ob_get_clean();
}



function hyde_panels_get_sublayout($plugin, $layout_name, $sublayout_name) {
  //dpm("here");
  //dpm(func_get_args());
  $theme = hyde_get_theme();
  //dpm($theme);
  if (isset($theme->layouts[$sublayout_name])) {
    $layout = $theme->layouts[$sublayout_name];
    $layout['name'] = $sublayout_name;
    return hyde_panels_layout_merge_plugin($plugin, $layout);
  }
}
function hyde_panels_get_sublayouts($plugin, $layout_name) {
  //dpm(get_defined_vars());
  $layouts = array();
  //$layouts[$layout_name] = $plugin;
  $theme = hyde_get_theme();
  //dpm($theme);
  foreach ($theme->layouts as $name => $layout) {
    $layout['name'] = 'hyde_layouts:' . $name;
    $layouts['hyde_layouts:' . $name] = hyde_panels_layout_merge_plugin($plugin, $layout);
  }
  //dpm($layouts);
  return $layouts;
}

function hyde_panels_layout_merge_plugin($plugin, $layout) {
  $plugin['name'] = $layout['name'];
  $plugin['category'] = t('Hyde');
  $plugin['title'] = $layout['label'];
  $plugin['regions'] = $layout['regions'];
  $plugin['layout'] = $layout;
  return $plugin;
}


/*
 * Callback to provide a single stored flexible layout.
function panels_flexible_get_sublayout($plugin, $layout_name, $sublayout_name) {
  // Do not worry about caching; Panels is handling that for us.
  ctools_include('export');
  $item = ctools_export_crud_load('panels_layout', $sublayout_name);
  if ($item) {
    return panels_flexible_merge_plugin($plugin, $item);
  }
}

/**
 * Callback to provide all stored flexible layouts.

function panels_flexible_get_sublayouts($plugin, $layout_name) {
  $layouts[$layout_name] = $plugin;
  ctools_include('export');
  $items = ctools_export_load_object('panels_layout', 'conditions', array('plugin' => 'flexible'));
  foreach ($items as $name => $item) {
    $layouts['flexible:' . $name] = panels_flexible_merge_plugin($plugin, $item);
  }

  return $layouts;
}
*/
