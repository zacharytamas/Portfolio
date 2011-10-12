/**
* This is a class written for a client project using 
* Adobe AIR and Flex to create a desktop application.
* The client (and the software) handles sensitive 
* client financial documents and was concerned with 
* the security of the data, thus this class was written 
* as an encryption/decryption layer when loading/saving 
* files. 
*/

package models 
{

   import com.adobe.crypto.MD5;
   import com.hurlant.crypto.prng.ARC4;
   import com.hurlant.util.Hex;

   import flash.filesystem.File;
   import flash.filesystem.FileMode;
   import flash.filesystem.FileStream;
   import flash.net.registerClassAlias;
   import flash.utils.ByteArray;
   import flash.utils.Dictionary;
   import flash.utils.Endian;
   import flash.utils.IDataInput;
   import flash.utils.IDataOutput;

   import mx.collections.ArrayCollection;
   import mx.utils.ObjectProxy;


   /**
   * Class for saving and loading Actionscript 
   * objects to a file using ARC4 encryption.
   * 
   * @author Zachary Jones <zacharytamas@gmail.com>
   *         Adam Duren <adamduren@gmail.com>
   * @version 1.0
   */
   public class Persyst 
   {

      private var file:File;
      private var store_data:Dictionary = new Dictionary();
      private var encryptionStream:ByteArray = new ByteArray();
      private var key:ByteArray = new ByteArray();
      private var kerberos:ARC4;
      private var isExternal:Boolean;

      public function Persyst(filepath:String, isExternal:Boolean = false) 
      {
         
         // register the classes we're using, otherwise when Flex outputs in AMF
         // it would 'forget' what was what.
         registerClassAlias("flash.utils.Dictionary", Dictionary);
         registerClassAlias("mx.collections.ArrayCollection", ArrayCollection);
         registerClassAlias("mx.utils.ObjectProxy", ObjectProxy);
         registerClassAlias("flash.utils.ByteArray", ByteArray);
         registerClassAlias("Date", Date);
         
         if (isExternal) {
         file = new File(filepath);
         } else {
         file = File.applicationStorageDirectory.resolvePath(filepath);
         }
         
         // Adam built this thing using military grade encryption...
         key.writeBytes(Hex.toArray("9a9d38b090f38dd34e899409cf9078c9f12abfd844c76d00bf69faf901e55663"), 0, 32);
         loadStore();
         
      }

      /**
      * Loads the datastore from the file associated with this Persyst object.
      */
      private function loadStore():void 
      {
         if (file.exists) {
            var _stream:FileStream;
            _stream = new FileStream();
            _stream.open(file, FileMode.READ);
            _stream.endian = Endian.BIG_ENDIAN;
            readStore(_stream);
            _stream.close();
         } else {
            save();
         }
      }


      /**
      * Sets an object value in the Persyst object, overwriting the existing data.
      * 
      * @param key The key name to save the value in the hashtable.
      * @param value The data to be stored. Supports default datatypes plus any you've defined with registerDataType()
      * 
      * @return This Persyst object for daisychaining.
      */
      public function set(key:String, value:Object):Persyst 
      {
         store_data[key]=value;
         return this;
      }


      /**
      * Retrieves data from the Persyst object by given key name. Invalid keys return null.
      * 
      * @param key Key name to pull data from.
      * @return Object Data found at the given key, could realistically be anything.
      */
      public function get(key:String):* 
      {
         var value:Object;

         try {
            value = store_data[key];
         } catch (e:Error) {
            value = null;
         }

         return value;
      }


      /**
      * Accessor function for accessing the raw data from this Persyst Object.
      * 
      * @return Dictionary Persyst file's data.
      */
      public function dict():Dictionary 
      { 
         return store_data; 
      }


      /**
      * Returns the hash of the Peryst object's data.
      * 
      * @return String MD5 hash of object's data.
      */
      public function get hash():String 
      {
         var _stream:ByteArray = new ByteArray();
         _stream.writeObject(store_data);
         return MD5.hashBytes(_stream);
      }


      /**
      * Saves the Persyst data to file.
      */
      public function save():void 
      {
         var _stream:FileStream;
         _stream = new FileStream();
         _stream.open(file, FileMode.WRITE);
         _stream.endian = Endian.BIG_ENDIAN;
         
         writeStore(_stream);
         _stream.close();
      }


      /**
      * Given an IDateOutput object writes Persyst's data to the file using ARC4 encryption.
      * 
      * @param file IDataOutput object to write to
      */
      private function writeStore(file:IDataOutput):void 
      {
         kerberos = new ARC4(key);
         encryptionStream.writeObject(store_data);
         kerberos.encrypt(encryptionStream);
         file.writeObject(encryptionStream);
         encryptionStream.clear();
      }

      /**
      * Given raw data input reads data from a Persyst file and loads it into the object.
      * 
      * @param raw IDataInput object to read.
      */
      private function readStore(raw:IDataInput):void 
      {
         try {
            kerberos = new ARC4(key);
            encryptionStream = raw.readObject() as ByteArray;
            kerberos.decrypt(encryptionStream);

            store_data = encryptionStream.readObject() as Dictionary;
            encryptionStream.clear();
         } catch (error:RangeError) {
            store_data = new Dictionary();
         }
      }
   }
}