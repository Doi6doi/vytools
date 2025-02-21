make {

   import { Dox; }
   
   init {
      $ixt := "index.txt";
      $ixh := "index.html";
      $rm  := "../README.md";
      $int := "install.txt";
      $inh := "Install.html";
      Dox.set("linkHead","https://doi6doi.github.io/vytools/");
      Dox.set("linkTail",".html");
   }

   target {

      /// build documentation
      build {
         index();
         install();
      }

      /// purge generated files
      clean {
         purge( [$rm,$inh,$ixh] );
      }
   }

   function {

      /// build index.html and README.md
      index() {
         foreach ( f | [$ixh, $rm] ) {
            if ( older( f, $ixt ))
               Dox.build( f, $ixt );
         }
      }

      /// build Install.html
      install() {
         if ( older( $inh, $int ))
            Dox.build( $inh, $int );
      }
   }

}
