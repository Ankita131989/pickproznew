<?php
/**
 * @file
 * Template fow diplaying TinyPass enabled terms
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
?>

<br/>

<div class="help">
  <p>
		The TinyPass can be enabled for specific terms in a vocabulary.  Choose an already
		defined taxonomy term and configure the TinyPass options for that term.  This feature will allow
		site owners to restrict access to all content that has been configured with that specific tag
  </p>
  <p>
		Enabled tags/terms will work in concert with regular TinyPass node configurations.  If a node is being displayed
		that has both term and node TinyPass configurations the term configuration will be displayed as the 'Upsell' in the ticket.
  </p>
</div>

<p>
<?php echo  l(t("Configure a new tag"), 'admin/content/tinypass/create_tag'); ?>
</p>

<table>

  <thead>
    <tr>
      <th width="30px">ID</th>
      <th width="30px"><?php echo t("Enabled"); ?></th>
      <th width="100px"><?php echo t("Tag/Term"); ?></th>
      <th><?php echo t("Options"); ?></th>
      <th width="30px">Operations</th>
  </thead>
  <tbody>

    <?php
    $i = 0;
    foreach ($rows as $row) {

      $oe = "even";
      if ($i++%2==0)
        $oe = "odd";

      echo "<tr class='$oe'>";
      echo "<td>{$row['meta_id']}</td>";
      echo "<td>{$row['enabled']}</td>";
      echo "<td>" . l($row['term'], "taxonomy/term/" . $row['tid']) . "</td>";
      echo "<td>{$row['details']}</td>";
      echo "<td><a href='" . url("admin/content/tinypass/") . "{$row['meta_id']}/edit_tag" . "'>" . t("edit") . "</a>";
      echo "&nbsp; <a onclick=\"return confirm('". t("Are you sure you want to remove these settings") ."');\" href='" . url("admin/content/tinypass/") . "{$row['meta_id']}/remove_tag" . "'>" . t('remove') . "</a></td>";
      echo "</tr>";

    } ?>

  </tbody>
</table>