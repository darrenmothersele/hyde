<?php

// Plugin definition
$plugin = array(
  'title' => t('Hyde Generated Box Styles'),
  'render pane' => 'hyde_box_styles_render_pane',

  'get child' => 'hyde_box_styles_get_substyle',
  'get children' => 'hyde_box_styles_get_substyles',
);

function theme_hyde_box_styles_render_pane($vars) {
  $content = $vars['content']->content;
  $title = $vars['content']->title;
  //dpm($vars);
  ob_start();
  eval('?>' . $vars['style']['style']['template'] . '<?php ');
  return ob_get_clean();
  return render();
}


function hyde_box_styles_merge_plugin($plugin, $style) {
  $plugin['name'] = 'hyde_box:' . $style['name'];
  $plugin['style'] = $style;
  $plugin['title'] = $style['label'];
  $plugin['weight'] = 0;
  return $plugin;
}

function hyde_box_styles_get_substyle($plugin, $style_name, $substyle_name) {
  $theme = hyde_get_theme();
  if (isset($theme->box_styles[$substyle_name])) {
    return hyde_box_styles_merge_plugin($plugin, $theme->box_styles[$substyle_name]);
  }
}
function hyde_box_styles_get_substyles($plugin, $style_name) {
  $styles = array();
  $theme = hyde_get_theme();
  foreach ($theme->box_styles as $name => $style) {
    $styles['hyde_box:' . $name] = hyde_box_styles_merge_plugin($plugin, $style);
  }
  return $styles;
}




/*
 * Callback to provide a single stored stylizer style.

function panels_stylizer_get_substyle($plugin, $style_name, $substyle_name) {
  // Do not worry about caching; Panels is handling that for us.
  ctools_include('export');
  $item = ctools_export_crud_load('stylizer', $substyle_name);
  if ($item) {
    return panels_stylizer_merge_plugin($plugin, $item);
  }
}

/*
 * Callback to provide all stored stylizer styles.

function panels_stylizer_get_substyles($plugin, $style_name) {
  $styles[$style_name] = $plugin;
  ctools_include('export');
  ctools_include('stylizer');
  $items = ctools_export_crud_load_all('stylizer');
  foreach ($items as $name => $item) {
    $base = ctools_get_style_base($item->settings['style_base']);
    if ($base && $base['module'] == 'panels') {
      $styles['stylizer:' . $name] = panels_stylizer_merge_plugin($plugin, $item);
    }
  }

  return $styles;
}
*/
