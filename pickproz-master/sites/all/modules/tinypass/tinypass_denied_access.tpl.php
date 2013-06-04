<?php
/**
 * @file
 * Template for displaying TinyPass button and messagning when access is denied
 */
/**
 * Tinypass
 * ------------------------------------------------------------------------
 * @author    TinyPass, Inc
 * @copyright Copyright (C) 2011 tinypass.com. All Rights Reserved.
 * @license - http://www.gnu.org/licenses/lgpl-3.0.html GNU/LGPL
 * Websites: http://tinypass.com
 * Technical Support: http://developer.tinypass.com
 */
/**
 * Template variables
 *
 * $button_html - the tinypass button
 * $access_message - the message provided in the TinyPass Configuration
 */
?>
<br/>
<div class="tinypass_button_holder">
  <div class="tinypass_access_message"><?php echo $access_message ?></div>
  <?php echo $button_html ?>
</div>