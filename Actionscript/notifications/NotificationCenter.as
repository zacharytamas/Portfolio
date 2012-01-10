package zlib.notifications
{
   import flash.utils.Dictionary;
   
   import mx.collections.ArrayList;
   import mx.core.UIComponent;
   import mx.utils.UIDUtil;

   /**
    * NotificationCenter.as
    * 
    * @author Zachary Jones <zacharytamas@gmail.com>
    * @version 1.0
    * 
    * A notification center for inter-application notifications 
    * between components. Using this system instead of Flex 
    * events is more efficient and also allows non-view components 
    * to use notifications. Only Flex View subclasses may dispatch 
    * events, which means other types of classes/methods would 
    * have to get other view components to dispatch events for 
    * them. Regardless, using a singleton NotificationCenter class 
    * allows for direct access to the center, whereas Flex Events 
    * would have to propagate and bubble up through the view 
    * hierarchy before they reached the handler. 
    * 
    */
   public class NotificationCenter
   {
      
      private static var _default:NotificationCenter = null;
      
      private var _observersForName:Dictionary  = new Dictionary();
      private var _observersForSender:Dictionary = new Dictionary();
      
      public function NotificationCenter()
      {
         trace("A NotificationCenter was created.");
      }
      
      
      /**
       * @todo 
       */
      public static function defaultCenter():NotificationCenter
      {
         if (!_default) {
            _default = new NotificationCenter();
         }
         return _default;
      }
      
      /**
       * @todo 
       */
      public function addObserverForName(observer:INotificationObserver,
         notificationName:String):Boolean
      {
         var success:Boolean = true;
         
         try {
            if (!_observersForName[notificationName]) {
               _observersForName[notificationName] = new ArrayList();
            }
            
            if (_observersForName[notificationName].getItemIndex(observer) == -1) {
               _observersForName[notificationName].addItem(observer);
            }
         } catch (error:Error) {
            success = false;
         }
         
         return success;
      }
      
      public static function AddObserverForName(observer:INotificationObserver,
         notificationName:String):Boolean
      {
         return NotificationCenter.defaultCenter().addObserverForName(observer, notificationName);
      }
      
      
      /**
       * @todo 
       */
      public function addObserverForSender(observer:INotificationObserver,
         notificationSender:Object):Boolean
      {
         var uniqueID:String = UIDUtil.getUID(notificationSender);
         var success:Boolean = false;
         
         try {
            if (!_observersForSender[uniqueID]) {
               _observersForSender[uniqueID] = new ArrayList();
            }
            
            if (_observersForSender[uniqueID].getItemIndex(observer) == -1) {
               _observersForSender[uniqueID].addItem(observer);
            }
         } catch (error:Error) {
            success = false;
         }
         
         return success;
      }
      
      public static function AddObserverForSender(observer:INotificationObserver,
         notificationSender:Object):Boolean
      {
         return NotificationCenter.defaultCenter().addObserverForSender(observer, notificationSender);
      }
      
      
      /**
       * @todo 
       */
      public function postNotification(notification:Notification):Boolean
      {
         
         var senderID:String = UIDUtil.getUID(notification.notificationSender);
         var observers:ArrayList = new ArrayList();
         
         if (_observersForSender[senderID]) {
            observers.addAll(_observersForSender[senderID]);
         }
         
         if (_observersForName[notification.notificationName]) {
            observers.addAll(_observersForName[notification.notificationName]);
         }
         
         for each (var observer:INotificationObserver in observers.toArray()) {
            observer.didReceiveNotification(notification);
         }
         
         if (observers.length == 0) {
            trace(notification.notificationName + ": An unobserved notification was posted. Poor guy, nobody even noticed.");
            // Returns false if there were not any observers for thie notification. 
            // Not fatal, just a heads-up.
            return false; 
         } else {
            // Returns true because there were observers.
            return true;
         }
         
      }
      
      public static function PostNotification(notification:Notification):Boolean
      {
         return NotificationCenter.defaultCenter().postNotification(notification);
      }
      
      
   }
}