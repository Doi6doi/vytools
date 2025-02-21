make {

   import { Dox; }
   
   init {
      $ixt := "index.txt";
      $ixh := "index.html";
      $rm  := "../README.md";
      $int := "install.txt";
      $inh := "Install.html";
      $base := "https://doi6doi.github.io/vytools/";
      links(true);
      Dox.set("style","style.css");
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
         links(true);
         if ( older( $rm, $ixt ))
            Dox.build( $rm, $ixt );
         links(false);
         if ( older( $ixh, $ixt ))
            Dox.build( $ixh, $ixt );
      }

      /// build Install.html
      install() {
         links(false);
         if ( older( $inh, $int ))
            Dox.build( $inh, $int );
      }

      /// turn full links on or off
      links(x) {
         if (x) {
            Dox.set("linkHead",$base);
            Dox.set("linkTail",".html");
         } else {
            Dox.set("linkHead",null);
            Dox.set("linkTail",null);
         }
      }

   }

}
