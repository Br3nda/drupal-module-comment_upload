<?php
// $Id: comment_upload_views.inc,v 1.1.2.1 2008/05/04 12:39:05 heine Exp $

/**
 * This file implements views functionality for the comment_upload.module.  This is a somewhat modified
 * version of the views_upload.inc file that comes with the views module.
 */

function _comment_upload_views_tables() {
  $tables['comment_upload_files'] = array(
    'name' => 'comment_upload_files',
    'type' => 'inner',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid',
      ),
      'right' => array(
        'field' => 'nid'
      ),
    ),
    'fields' => array(
      'fid' => array(
        'name' => t('Comment upload: File id'),
        'sortable' => true,
        'help' => t('File Id which represents the file.'),
      ),
      'all_files' => array(
        'name' => t('Comment upload: All files'),
        'notafield' => true,
        'handler' => array(
          'views_handler_comment_upload_all_files' => t('All files'),
          'views_handler_comment_upload_listed_files' => t('Listed files')),
        'option' => array(
          '#type' => 'select',
          '#options' => array(
            // kept original behind-the-scenes names link/nolink so patch to add description wouldn't affect existing Views
            'link' => t('File names with links'),
            'nolink' => t('File names without links'),
            'linkdesc' => t('File descriptions with links'),
            'nolinkdesc' => t('File descriptions without links'),
          )),
        'sortable' => false,
        'help' => t('Display all attached files in one field.'),
      ),
      'filename' => array(
        'name' => t('Comment upload: File name'),
        'handler' => array(
          'views_handler_file_filename' => t('Plain'),
          'views_handler_comment_upload_filename_download' => t('With download link'),
        ),
        'sortable' => true,
        'addlfields' => array('filepath'),
        'option' => 'string',
        'help' => t('Display file name'),
      ),
      'filepath' => array(
        'name' => t('Comment upload: File path'),
        'sortable' => false,
        'help' => t('Display path to File.'),
      ),
      'filesize' => array(
        'name' => t('Comment upload: File size'),
        'handler' => 'views_handler_file_size',
        'sortable' => true,
        'help' => t('Display the size of the associated file.'),
      ),
      'filemime' => array(
        'name' => t('Comment upload: File mime type'),
        'sortable' => true,
        'help' => t('Display the mime type of the associated file.'),
      ),
    ),
    'filters' => array(
      'filename' => array(
        'name' => t('Comment upload: File name'),
        'operator' => 'views_handler_operator_like',
        'handler' => 'views_handler_filter_like',
        'help' => t('This filter allows nodes to be filtered by the name of files attached to comments.'),
      ),
      'filepath' => array(
        'name' => t('Comment upload: File path'),
        'operator' => 'views_handler_operator_like',
        'handler' => 'views_handler_filter_like',
        'help' => t('This filter allows nodes to be filtered by the path of files attached to comments.'),
      ),
      'filesize' => array(
        'name' => t('Comment upload: File size'),
        'operator' => 'views_handler_operator_gtlt',
        'help' => t('This filter allows nodes to be filtered by the file size of files attached to comments.'),
      ),
      'filemime' => array(
        'name' => t('Comment upload: File mime type'),
        'operator' => 'views_handler_operator_like',
        'handler' => 'views_handler_filter_like',
        'help' => t('This filter allows nodes to be filtered by the mime type of files attached to comments.'),
      ),
      'fid' => array(
        'name' => t('Comment upload: Has file downloads'),
        'operator' => array('=' => t('Exists')),
        'list' => 'views_handler_operator_yesno',
        'list-type' => 'select',
        'handler' => 'views_handler_comment_upload_filter_fid_exist',
        'help' => t('Filter whether the node has comment(s) with files for download'),
      ),
      'list' => array(
        'name' => t('Comment upload: File listed in file downloads'),
        'operator' => array('=' => t('Equals')),
        'list' => 'views_handler_operator_yesno',
        'list-type' => 'select',
        'help' => t('Filter whether the node has comment(s) with files that are listed in downloads'),
      ),
    ),
    'sorts' => array(
      'fid' => array(
        'name' => t('Sort by Comment upload file Id'),
        'help' => t('Sort files attached to comments by file Id '),
      ),
      'filename' => array(
        'name' => t('Comment upload: File name'),
        'help' => t('Sort by file name'),
      ),
      'filesize' => array(
        'name' => t('Comment upload: File size'),
        'help' => t('Sort by file size.'),
      ),
      'filemime' => array(
        'name' => t('Comment uplaod: Mime type'),
        'help' => t('Sort by mime type.'),
      ),
    ),
  );
  return $tables;
}

function views_handler_comment_upload_filename_download($fieldinfo, $fielddata, $value, $data) {
  $link_text = $fielddata['options'] ? $fielddata['options'] : $value;
  return $value ? l($link_text, check_url(file_create_url($data->comment_upload_files_filepath))) : '';
}

function views_handler_comment_upload_filter_fid_exist($op, $filter, $filterdata, &$query) {
  switch ($op) {
    case 'handler':
      $query->ensure_table('comment_upload_files');
      if ($filter['value']) {
        $query->add_where('comment_upload_files.fid');
      }
      else {
        $query->add_where('ISNULL(comment_upload_files.fid)');
      }
  }
}

/**
 * Display all files attached to a given node via comment_upload.
 */
function views_handler_comment_upload_all_files($fieldinfo, $fielddata, $value, $data, $listed = FALSE) {
  $and = $listed ? ' AND list = 1' : '';

  $links = array();

  $result = db_query("SELECT * FROM {comment_upload_files} WHERE nid = %d $and", $data->nid);
  while ($file = db_fetch_object($result)) {
    // link/nolink use file filename; linkdesc/nolinkdesc use file description
    if ($fielddata['options'] == 'link' || $fielddata['options'] == 'nolink') {
      $display_string = $file->filename;
    } else {
      $display_string = $file->description;
    }
    if ($fielddata['options'] == 'nolink' || $fielddata['options'] == 'nolinkdesc') {
      $links[] = check_plain($display_string);
    } else {
      $links[] = l($display_string, check_url(file_create_url($file->filepath)));
    }
  }
  return implode(' | ', $links);
}

function views_handler_comment_upload_listed_files($fieldinfo, $fielddata, $value, $data) {
  return views_handler_comment_upload_all_files($fieldinfo, $fielddata, $value, $data, TRUE);
}
