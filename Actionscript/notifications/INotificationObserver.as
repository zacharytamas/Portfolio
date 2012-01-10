package zlib.notifications
{
   /**
    * INotificationObserver.as
    * @author Zachary Jones <zacharytamas@gmail.com>
    * @version 1.0
    * 
    * An interface which objects which intend to interact 
    * with the NotificationCenter must adhere to.
    *  
    */
   public interface INotificationObserver
   {
      
      /**
       * The delegated action handler for objects which 
       * have subscribed to events.
       */
      function didReceiveNotification(notification:Notification):void;
      
   }
}