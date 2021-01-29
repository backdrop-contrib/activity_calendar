Activity Calendar
=================

Activity Calendar module creates an activity calendar heatmap for each user of
your Backdrop website, similar to GitHub contribution calendar.

On module's administration page you can configure such *Calendar settings* as
monthly or yearly output format, lightest and darkest colors for highlighting
calendar cells, point thresholds for changing the color of users daily activity.

Under the *Activity points* section you can select actions that will increment
activity points, such as creating nodes or submitting comments, limit content
types and set how many points users get by creating new nodes and comments.


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
  in the required form fields.

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
- Cal-HeatMap library is eveloped by [Wan Qi Chen](https://github.com/wa0x6e) and
  is licensed under the [MIT License](https://opensource.org/licenses/MIT).

License
-------

This project is GPL v3 software.
See the LICENSE.txt file in this directory for complete text.
