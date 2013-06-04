<?php

global $base_url;

$author_uid        = $view->result[0]->node_uid;
$user_pic =db_query("SELECT fm.uri FROM `file_usage` as fu right join file_managed as fm on(fm.fid=fu.fid) where fm.uid='$author_uid' and fu.type='profile2'");
foreach ($user_pic as $row)
{

        $uri = $row->uri;
        $or_uir = file_create_url($uri);
        $alias_user = db_query("SELECT alias FROM `url_alias` where source='user/$author_uid'");
        foreach($alias_user as $ali_rows)
        {
                $use_alias = $ali_rows->alias;
                
                echo "<a href='$base_url/$use_alias'><img src='$or_uir' width='100px' height='100'></a>"; 
           
        }
        
       
}



?>
