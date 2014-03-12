<?php


class views_plugin_style_hyde_generated extends views_plugin_style {

  function options(&$options) {
    $theme = hyde_get_theme();
    $list_styles = array();
    foreach ($theme->list_styles as $name => $label) {
      $list_styles[$name] = $label['label'];
    }
    if (!empty($list_styles)) {
      reset($list_styles);
      $options['style_name'] = key($list_styles);
    }
    else {
      $options['style_name'] = '';
    }
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $theme = hyde_get_theme();
    $list_styles = array();
    foreach ($theme->list_styles as $name => $label) {
      $list_styles[$name] = $label['label'];
    }
    $form['style_name'] = array(
      '#type' => 'select',
      '#title' => t('Style name'),
      '#default_value' => $this->options['style_name'],
      '#options' => $list_styles,
    );
  }
}
