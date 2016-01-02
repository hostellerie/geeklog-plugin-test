<?php

/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | Test Plugin 1.0                                                           |
// +---------------------------------------------------------------------------+
// | functions.inc                                                             |
// |                                                                           |
// | This file does two things: 1) it implements the necessary Geeklog Plugin  |
// | API methods and 2) implements all the common code needed by this plugin.  |
// +---------------------------------------------------------------------------+
// | Copyright (C) 2016 by the following authors:                              |
// |                                                                           |
// | Authors: Ben - hostellerie DOT org AT gmail DOT com                       |
// +---------------------------------------------------------------------------+
// | Created with the Geeklog Plugin Toolkit.                                  |
// +---------------------------------------------------------------------------+
// |                                                                           |
// | This program is free software; you can redistribute it and/or             |
// | modify it under the terms of the GNU General Public License               |
// | as published by the Free Software Foundation; either version 2            |
// | of the License, or (at your option) any later version.                    |
// |                                                                           |
// | This program is distributed in the hope that it will be useful,           |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of            |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
// | GNU General Public License for more details.                              |
// |                                                                           |
// | You should have received a copy of the GNU General Public License         |
// | along with this program; if not, write to the Free Software Foundation,   |
// | Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           |
// |                                                                           |
// +---------------------------------------------------------------------------+

/**
* @package Test
*/

if (strpos(strtolower($_SERVER['PHP_SELF']), 'functions.inc') !== false) {
    die ('This file can not be used on its own.');
}

/**
 * Language file include
 */
$plugin_path = $_CONF['path'] . 'plugins/test/';
$langfile = $plugin_path . 'language/' . $_CONF['language'] . '.php';

if (file_exists($langfile)) {
    require_once $langfile;
} else {
    require_once $plugin_path . 'language/english.php';
}

// +---------------------------------------------------------------------------+
// | Geeklog Plugin API Implementation                                         |
// +---------------------------------------------------------------------------+

/**
* Returns the item(s) for this plugin that should appear on the main menu
*
* NOTE: this MUST return the url/value pairs in the following format
* $<arrayname>[<label>] = <url>
*
* @return   mixed   menu entry, or boolean false if disabled / hidden
*
*/
function plugin_getmenuitems_test()
{
    global $_CONF, $LANG_TEST_1;

    $url = $_CONF['site_url'] . '/test/index.php';
    $menuitems[$LANG_TEST_1['plugin_name']] = $url;

    return $menuitems;
}

/**
* Implements a [test:] autotag.
*
* @param    string  $op         operation to perform
* @param    string  $content    item (e.g. story text), including the autotag
* @param    array   $autotag    parameters used in the autotag
* @param    mixed               tag names (for $op='tagname') or formatted content
*
*/
/*
function plugin_autotags_test($op, $content = '', $autotag = '')
{
    global $_CONF, $_TABLES, $LANG_TEST_1;

    if ($op == 'tagname') {
        return 'test';
    } elseif ($op == 'parse') {
        $tid = COM_applyFilter($autotag['parm1']);
        if (! empty($tid)) {
            $url = $_CONF['site_url'] . '/test/index.php?item=' . $tid;
            if (empty($autotag['parm2'])) {
                $linktext = 'Test'; // or get title from db
            } else {
                $linktext = $autotag['parm2'];
            }
            $link = COM_createLink($linktext, $url);
            $content = str_replace($autotag['tagstr'], $link, $content);
        }

        return $content;
    }
}
*/

/**
* Return plugin entry for "Command and Control" (moderation.php)
*
* @return  array  Array containing (plugin name, admin url, url of plugin icon)
*
*/
function plugin_cclabel_test()
{
    global $_CONF, $LANG_TEST_1;

    $retval = array();
    if (SEC_hasRights('test.admin')) {
        $retval = array($LANG_TEST_1['plugin_name'],
            $_CONF['site_admin_url'] . '/plugins/test/index.php',
            plugin_geticon_test());
    }

    return $retval;
}

/**
* Return plugin entry for the Admins Only block
*
* @return array Array containing (plugin name, plugin admin url, # of plugin items or '')
*/
function plugin_getadminoption_test()
{
    global $_CONF, $LANG_TEST_1;

    if (SEC_hasRights('test.admin')) {
        return array($LANG_TEST_1['plugin_name'],
            $_CONF['site_admin_url'] . '/plugins/test/index.php', 0);
    }
}

/**
* Returns the URL of the plugin's icon
*
* @return   string      URL of the icon
*
*/
function plugin_geticon_test()
{
    global $_CONF;

    // popular alternative location:
    // return $_CONF['site_url'] . '/test/images/test.png';

    return $_CONF['site_admin_url'] . '/plugins/test/images/test.png';
}

/**
 * Returns the current plugin code version
 *
 * @return string    plugin version
 */
function plugin_chkVersion_test()
{
    global $_CONF;

    require_once $_CONF['path'] . 'plugins/test/autoinstall.php';

    $inst_parms = plugin_autoinstall_test('test');

    return $inst_parms['info']['pi_version'];
}

/**
* Update the plugin
*
* @return   int     Number of message to display or true for success
*
*/
function plugin_upgrade_test()
{
    global $_CONF, $_TABLES;

    $installed_version = DB_getItem($_TABLES['plugins'], 'pi_version',
                                    "pi_name = 'test'");
    $code_version = plugin_chkVersion_test();
    if ($installed_version == $code_version) {
        // nothing to do
        return true;
    }

    require_once $_CONF['path'] . 'plugins/test/autoinstall.php';

    if (! plugin_compatible_with_this_version_test('test')) {
        return 3002;
    }

    // other update code goes here

    // update plugin version number
    $inst_parms = plugin_autoinstall_test('test');
    $pi_gl_version = $inst_parms['info']['pi_gl_version'];

    DB_query("UPDATE {$_TABLES['plugins']} SET pi_version = '$code_version', pi_gl_version = '$pi_gl_version' WHERE pi_name = 'test'");

    return true;
}

/**
* Automatic uninstall function for plugins
*
* This code is automatically uninstalling the plugin.
* It passes an array to the core code function that removes
* tables, groups, features and php blocks from the tables.
* Additionally, this code can perform special actions that cannot be
* foreseen by the core code (interactions with other plugins for example)
*
* @return   array   Plugin information
*
*/
function plugin_autouninstall_test()
{
    $out = array (
        /* give the name of the tables, without $_TABLES[] */
        'tables' => array(/* e.g. 'test' */),
        /* give the full name of the group, as in the db */
        'groups' => array('Test Admin'),
        /* give the full name of the feature, as in the db */
        'features' => array('test.admin'),
        /* give the full name of the block, including 'phpblock_', etc */
        'php_blocks' => array(),
        /* give all vars with their name */
        'vars' => array()
    );

    return $out;
}

?>
