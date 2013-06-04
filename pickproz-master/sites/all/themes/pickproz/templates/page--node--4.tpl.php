<div id="page" class="container">

  
  <?php print $messages; ?>
  

  <div id="columns">
    <div class="columns-inner clearfix">
      <div id="content-column">
        <div class="content-inner">
          <div id="main-content" role="main">
            <div id="content"><?php print render($page['content']); ?></div>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>
