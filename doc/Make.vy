make {

   import { Dox; }
   
   init {
      $ixt := "index.txt";
      $int := "install.txt";
      $rm  := "../README.md";
      $ins := "html/Install.html";
      Dox.set("linkHead","https://raw.githubusercontent.com/Doi6doi/vytools/main/doc/html/");
      Dox.set("linkTail",".html");
   }

   target {

      /// build documentation
      build {
         readme();
         install();
      }

      /// purge generated files
      clean {
         purge( [$rm,$ins] );
      }
   }

   function {

      /// build README.md
      readme() {
         if ( older( $rm, $ixt ) )
            Dox.build( $rm, $ixt );
      }

      /// build Install.html
      install() {
         if ( older( $ins, $int ))
            Dox.build( $ins, $int );
      }
   }

}
