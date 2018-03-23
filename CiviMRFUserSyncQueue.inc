<?php

class CiviMRFUserSyncQueue extends SystemQueue {
	
	/**
	 * @var contains the processed UIDS
	 */
	protected $processedUIDs = array();
	
	public function __construct($name) {
		parent::__construct($name);
		$this->processedUIDs = variable_get('civimrf_user_sync_processed_uids', array());
	}
	
	
	/**
	 * Returns the processed UIDs 
	 *
	 * @return array
	 */
	public function getProcessedUIDs() {
		return $this->processedUIDs;
	}
	
	/**
	 * Add a single uid or multiple uids
	 * 
	 * @param array|int $uids
	 */
	public function addProcessedUIDs($uids) {
		if (is_array($uids)) {
			$this->processedUIDs = array_merge($this->processedUIDs, $uids);
		} else {
			$this->processedUIDs[] = $uids;
		}
	}
	
	public function resetProcessedUIDs() {
		$this->processedUIDs = array();
	}
	
	public function deleteItem($item) {
		parent::deleteItem($item);
		if ($this->numberOfItems() == 0) {
			// Queue finished 
			$queues = module_invoke_all('cron_queue_info');
  		drupal_alter('cron_queue_info', $queues);
			$queue_info = $queues[$this->name];
			$callback = isset($queue_info['civimrf user sync finish callback']) ? $queue_info['civimrf user sync finish callback'] : false;
			if ($callback) {
				call_user_func($callback, $this->processedUIDs);
			}
			$this->processedUIDs = array();
		}
		variable_set('civimrf_user_sync_processed_uids', $this->processedUIDs);
	}
	
	
	
}
