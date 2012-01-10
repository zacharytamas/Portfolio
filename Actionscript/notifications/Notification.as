package zlib.notifications
{
   
   
   /**
     * Notification.as
     * @author Zachary Jones <zacharytamas@gmail.com>
     * @version 1.0 
     */
   public class Notification
   {
      private var _notificationName:String = null;
      private var _notificationSender:* = null;
      private var _notificationInfo:Object = null;
      
      public function Notification(name:String, notificationSender:* = null, 
         notificationInfo:Object = null)
      {
         _notificationName = name;
         _notificationSender = notificationSender;
         _notificationInfo = notificationInfo; 
      }
      
      
      /**
       * Creates new notification with the given name. 
       */
      public static function notificationWithName(notificationName:String):Notification
      {
         return new Notification(notificationName);
      }
      
      
      /**
       * @todo 
       */
      public static function notificationWithNameAndSender(notificationName:String,
         notificationSender:*):Notification
      {
         return new Notification(notificationName, notificationSender);
      }
      
      
      /**
       * @todo 
       */
      public static function notificationWithNameSenderAndInfo(notificationName:String,
         notificationSender:*, notificationInfo:Object):Notification
      {
         return new Notification(notificationName, notificationSender, notificationInfo);
      }
      
      
      /**
       * @todo 
       */
      public function get notificationName():String
      {
         return _notificationName;
      }
      
      
      /**
       * @todo 
       */
      public function get notificationSender():*
      {
         return _notificationSender;
      }
      
      
      /**
       * @todo 
       */
      public function get notificationInfo():Object
      {
         return _notificationInfo;
      }
      
      
   }
}