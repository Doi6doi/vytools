make {

   import { Dox; }
   
   init {
      $ixt := "index.txt";
      $ixh := "index.html";
      $rm  := "../README.md";
      $int := "install.txt";
      $inh := "Install.html";
      $cs  := "../vytools.h";
      $ch  := "C.html";
      $cps := "../vytools.hpp";
      $cph := "Cpp.html";
      $base := "https://doi6doi.github.io/vytools/";
      $purge := [$ixh,$rm,$inh,$ch,$cph];
      links(true);
      Dox.set("linkTail",".html");
      Dox.set("style","style.css");
   }

   target {

      /// build documentation
      build {
         index();
         install();
         c();
      }

      /// purge generated files
      clean {
         purge( $purge );
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

      c() {
         links(false);
         if ( older( $ch, $cs ))
            Dox.build( $ch, $cs );
      }

      /// turn full links on or off
      links(x) {
         if (x)
            Dox.set("linkHead",$base);
            else Dox.set("linkHead",null);
      }

   }

}
