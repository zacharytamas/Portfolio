package zlib.models
{
   import avmplus.getQualifiedClassName;
   
   import com.hurlant.crypto.prng.ARC4;
   import com.hurlant.util.Hex;
   
   import flash.filesystem.File;
   import flash.filesystem.FileMode;
   import flash.filesystem.FileStream;
   import flash.utils.ByteArray;
   import flash.utils.Endian;
   
   import mx.utils.ObjectUtil;

   /**
    * ZLDataStore.as
    * 
    * @author Zachary Jones <zacharytamas@gmail.com>
    * @version 1.0
    */
   public class ZLDataStore
   {
      
      private static var _default:ZLDataStore = null;
      
      /**
       * The class of the root object this DataStore contains. 
       */
      protected var rootObjectClass:Class;
      
      protected var kerberos:ARC4;

      public var encryptFiles:Boolean = false;
      
      public function ZLDataStore()
      {
         registerClasses();
         
         if (encryptFiles) {
            var key:ByteArray = new ByteArray();
            key.writeBytes(Hex.toArray(encryptionKey()), 0, 32);    
            kerberos = new ARC4(key);
            trace("Kerberos was created.");
         }

      }
      
      /**
       * Returns the default Data Store for this class. If one doesn't 
       * exist it will be created. This avoids your application creating 
       * and destroying identical DataStores everytime you need to write 
       * or open a file, plus allows convenient access to it anywhere 
       * by just importing the class.
       */
      public static function defaultStore():ZLDataStore
      {
         if (!_default) {
            _default = new ZLDataStore();
         }
         return _default;
      }
      
      /**
       * Returns the encryption key used by the data store. 
       * Override this when subclassing, otherwise you have 
       * a potentially vulnerable encryption. 
       */
      protected function encryptionKey():String
      {
         trace("ZLDataStore encryption key was read.");
         return "9a9d38b090f38dd34d899409cf9078c9f12abfd844c76d00bf69faf90aaaaaaa";
      }
      
      /**
       * Registers any custom classes so that when they are 
       * encoded their identity is encoded with them. Otherwise, 
       * they would be encoded as Anonymous objects. Override 
       * this and use registerClassAlias() to register any custom 
       * classes your DataStore may be encoding.
       */
      public function registerClasses():void
      {
         
      }
      
      /**
       * Encodes almost any kind of object and returns it as a ByteArray. 
       * If you need to encode a custom class make sure you've registered 
       * it by overriding registerClasses() and using registerClassAlias()
       * NOTE: It has not been encrypted when it is returned here. 
       */
      public function encodeObject(object:*):ByteArray
      {
         var encodingStream:ByteArray = new ByteArray();
         encodingStream.writeObject(object);
         trace("Object was encoded.");
         return encodingStream;
      }
      
      /**
       * 
       */
      public function encryptStream(stream:ByteArray):ByteArray
      {
         kerberos.encrypt(stream);
         trace("Object was encrypted.");
         return stream;
      }
      
      /**
       * Accepts a root object and a file path to write that 
       * object to. If encryption is enabled the object will 
       * also be encrypted before written out to file.
       */
      public function writeToFile(object:*, filePath:String):void
      {

         var stream:ByteArray = encodeObject(object);
         
         if (encryptFiles) {
            stream = encryptStream(stream);
         }
         
         var file:FileStream = new FileStream();
         file.open(new File(filePath), FileMode.WRITE);
         file.endian = Endian.BIG_ENDIAN;
         file.writeObject(stream);
         file.close();
         trace("Object was written to file.");
         
      }
      
      
      public function decodeObject(stream:ByteArray):*
      {
         try {
            trace("Trying to decode object...");
            return (stream.readObject() as rootObjectClass);   
         } catch (error:RangeError) {
            trace("Object couldn't be decoded.");
            return null;
         }
         
      }
      
      
      public function decryptObject(rawStream:ByteArray):ByteArray
      {
         kerberos.decrypt(rawStream);
         trace("Stream was decrypted.");
         return rawStream;
      }
      
      
      public function openFromFile(filePath:String):*
      {
         var file:FileStream = new FileStream();
         file.open(new File(filePath), FileMode.READ);
         file.endian = Endian.BIG_ENDIAN;
         
         var stream:ByteArray = file.readObject() as ByteArray;
         file.close();
         
         if (encryptFiles) {
            decryptObject(stream);
         }
         
         var obj:* = decodeObject(stream);
         trace("Object was read from file.");
         return obj;
         
      }
      
      
   }
}