Activity Calendar
=================

Activity Calendar module creates an activity calendar heatmap for each user of
your Backdrop website, similar to GitHub contribution calendar.

On module's administration page you can configure such *Calendar settings* as
monthly or yearly output format, lightest and darkest colors for highlighting
calendar cells, point thresholds for changing the color of users' daily activity.

Under the *Activity points* section you can select actions that will increment
activity points, such as creating nodes or submitting comments, limit content
types and set how many points users get by creating new nodes and comments per
content type.

If *Flag Activity* sub-module is enabled, you can configure number of points
assigned for each flagging. 

Other modules can implement `hook_activity_calendar()` that should return array
of `module_name` string and `$user_activity` sub-array, which in turn should have
Unix timestamps as array keys and number of points for each timestamp entry. [For 
Example](https://github.com/backdrop-contrib/activity_calendar/blob/1.x-1.x/modules/flag_activity/flag_activity.module):


```PHP

/**
 * @file
 * Flag activity module.
 */

/**
 * Implements hook_calendar().
 */
function flag_activity_calendar() {
  $config_flag_points = config_get('activity_calendar.settings', 'flag_activity');
  $flag_points = ($config_flag_points) ? $config_flag_points : 1;

  if (arg(0) == 'user') {
    // Collect all flag points as the hook is called from user page.
    $flags = db_select('flagging', 'f')
    ->fields('f', array('flagging_id', 'timestamp'))
    ->condition('f.uid', arg(1))
    ->execute()->fetchAll();
  }
  else {
    // Collect flag points only for the current month.
    global $user;
    $flags = db_select('flagging', 'f')
    ->fields('f', array('flagging_id', 'timestamp'))
    ->condition('f.uid', $user->uid)
    ->condition('f.timestamp', strtotime('first day of this month'), '>=')
    ->execute()->fetchAll();
  }

  if (!empty($flags)) {
    foreach ($flags as $flag) {
      $user_activity[$flag->timestamp] = intval($flag_points);
    }
    $result['module_name'] = 'Flag Activity';
    $result['user_activity'] = $user_activity;
  }
  return $result;
}
```

![Activity Calendar](https://raw.githubusercontent.com/backdrop-contrib/activity_calendar/1.x-1.x/activity_calandar.png
)

Requirements
------------

This module relies on [Cal-HeatMap library](https://cal-heatmap.com). No need to
download anything as the library files are already bundled into the module.  

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules or run the following oneliner command
  in case if [Brush](https://github.com/backdrop-contrib/brush) is installed:

  ```
  brush en activity_calendar -y
  ```
- Visit the configuration page under Administration > Configuration > User
  accounts > Activity calendar (admin/config/people/activity_calendar) and fill
  in the form per your preferences.

- Go to any user's profile page and enjoy checking their activity calendar.

Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/activity_calendar/issues.

Current Maintainers
-------------------

- Created for Backdrop and maintained by [Alan Mels](https://github.com/alanmels).

Credits
-------

- Sponsored by [AltaGrade](https://www.altagrade.com) - a Backdrop and Drupal
  hosting provider.
- Cal-HeatMap library is developed by [Wan Qi Chen](https://github.com/wa0x6e) and
  is licensed under the [MIT License](https://opensource.org/licenses/MIT).

License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
