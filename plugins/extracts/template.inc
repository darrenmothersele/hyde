<?php

$plugin = array(
  'title' => t('Template extraction'),
  'selector' => '[x-drupal-type=template]',
  'callback' => '_hyde_template_extraction',
);


function _hyde_template_replace_var($name, $el, $layout_var = FALSE) {
  $el->first()->removeAttr('x-drupal-var')->removeAttr('x-drupal-name');

  if ($el->hasAttr('x-drupal-no-wrap')) {
    if ($layout_var) {
      $el->before("{{[[PHP_OPEN print \$content['" . $name . "']; PHP_CLOSE]]}}");
    }
    else if ($el->hasAttr('x-drupal-php')) {
      $php = $el->attr('x-drupal-php');
      $el->before("{{[[PHP_OPEN print " . $php . "; PHP_CLOSE]]}}");
      $el->removeAttr('x-drupal-php');
    }
    else {
      $el->before('{{[[PHP_OPEN print $' . $name . '; PHP_CLOSE]]}}');
    }
    $el->remove();
  }
  else {
    if ($layout_var) {
      $el->html("{{[[PHP_OPEN print \$content['" . $name . "']; PHP_CLOSE]]}}");
    }
    else if ($el->hasAttr('x-drupal-php')) {
      $php = $el->attr('x-drupal-php');
      $el->html("{{[[PHP_OPEN print " . $php . "; PHP_CLOSE]]}}");
      $el->removeAttr('x-drupal-php');
    }
    else {
      $el->html('{{[[PHP_OPEN print $' . $name . '; PHP_CLOSE]]}}');
    }
  }
}

function _hyde_template_extraction($element, $theme) {
  // hyde_dpm($element->html(), '0', TRUE);

  $name = $element->attr('x-drupal-name');
  drupal_set_message("Template extraction for {$name}.tpl.php");

  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name');
  /*
  TODO: allow more general customisation of preprocess
  if ($element->hasAttr('data-drupal-preprocess')) {
    $element->removeAttr('data-drupal-preprocess');
    $preprocess = "\n\nfunction generated_preprocess_{$name}(&\$variables) {\n";

    foreach ($element->find('[data-drupal-preprocess-attr]') as $el) {
      $preprocess_attr_name = $el->attr('data-drupal-preprocess-attr');
      if (empty($preprocess_attr_name)) {
        $preprocess_attr_name = $el->tag() . '_attributes';
      }
      $preprocess .= "  \$variables['" . $preprocess_attr_name . "'] = ";
      $preprocess_attr = $el->attr();
      unset($preprocess_attr['data-drupal-preprocess-attr']);
      $preprocess_str = var_export($preprocess_attr, TRUE);
      $preprocess_str = preg_replace("/^/m", "    ", $preprocess_str);
      $preprocess .= "drupal_attributes(\n" . $preprocess_str . "\n  );\n";
      $el->removeAttr('data-drupal-preprocess-attr');
    }

    $preprocess .= "}\n\n";
    drush_file_append_data("{$tmp}/template.php", $preprocess);
  }
  */

  if ($element->find('head')->size()) {
    $element->find('head')->append("\n{{[[PHP_OPEN print \$styles; PHP_CLOSE]]}}\n{{[[PHP_OPEN print \$scripts; PHP_CLOSE]]}}\n");
  }

  //hyde_dpm($element->html(), '1', TRUE);

  while ($element->find('[x-drupal-var]')->size()) {
    $el = $element->find('[x-drupal-var]')->first();
    $el_name = $el->attr('x-drupal-name');
    _hyde_template_replace_var($el_name, $el);
  }

  //hyde_dpm($element->html(), '2', TRUE);

/*  $dst = "{$theme->tmp_path}/templates/{$name}.tpl.php";
  if (!file_exists(dirname($dst))) {
    mkdir(dirname($dst), 0777, TRUE);
  }
  file_put_contents($dst, hyde_replace_php_tags($element->html()));
  */
  if ($element->hasAttr('x-drupal-no-wrap')) {
    $theme->templates[$name] = array('template' => hyde_replace_php_tags($element->innerHTML()));
  }
  else {
    $theme->templates[$name] = array('template' => hyde_replace_php_tags($element->html()));
  }
  //dpm($theme);
}
