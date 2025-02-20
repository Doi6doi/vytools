make {

   import { Git; }

   function init() {
      $name := "vytools";
      $url := "https://github.com/Doi6doi/vytools.git";
   }


   target {

      // download and/or build 
      default {
         init();
         if ( inDir() )
            build();
         else {
            download();
            make( $name );
         }
      }

      // download the project
      download {
         Git.clone( $url );
      }

      // build project
      build {
        echo("BUILD");
      }
   }

   function {
      inDir() {
         return exists("vytools.h");
      }
   }



}
