$Id: README.txt,v 1.1.2.1 2007/01/17 10:44:57 heine Exp $

Description
===========
Comment upload allows users to attach files to individual comments. If the attached
file is an image and the comment upload mode is set to "single", it's displayed inline in the
resulting comment. Otherwise, a hyperlink to the file is displayed after the comment's body.


Dependencies
============
Comment upload depends on the following core modules:

* Upload
* Comment


Installation & Configuration
============================
1. Copy all the folder in the archive to your module folder.
2. Enable comment upload on administer >> modules (admin/modules).
3. Configure the module
   * Enable "Attachments on comments" by editing content types on
     administer >> settings >> content types (admin/settings/content-types).
   * Choose on administer >> settings >> upload (admin/settings/upload)
     whether to have single or multiple uploads per comment.
     If you choose "single", you can choose whether to display images inline by selecting
     "Inline display" for the "Images on comments" setting.

All other settings are inherited from the Upload module.

Support
=======
If you require support, file a support request or post on the Drupal.org forum.

* Support request - <http://drupal.org/node/add/project-issue/comment_upload/support>.
* Drupal.org forum - <http://drupal.org/forum>.

Please search for answers before doing so however.

Bug reports & Feature requests
==============================
Please submit bug reports and feature requests to the issue tracker:

* Bug reports - <http://drupal.org/node/add/project-issue/comment_upload/bug>.
* Feature requests - <http://drupal.org/node/add/project-issue/comment_upload/feature>.

Please search for existing issues on <http://drupal.org/project/issues/37197>.

Current maintainer
==================
Heine Deelstra <http://drupal.org/user/17943>.

This started as chx's proof-of-concept project to demonstrate the new hooks in
Drupal 4.7's comment.module. Then Eaton got his hands on it and started hacking with it.

It's still very early in the development cycle, and will continue to grow.
