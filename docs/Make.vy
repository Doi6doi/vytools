make {

   init {
      $Dox := tool("Dox", {linkTail:".html", style:"style.css",
         omit:"VYT_EXPORT "});
   
      $ixx := "index.dox";
      $ixh := "index.html";
      $rm  := "../README.md";
      $inx := "install.dox";
      $inh := "Install.html";
      $cs  := "../vytools.h";
      $ch  := "C.html";
      $ds  := "../vytools_defs.h";
      $dh  := "Defs.html";
      $ps := "../vytools.hpp";
      $ph := "Cpp.html";
      $ts := "../vytools_tpl.hpp";
      $th := "Templates.html";
      $base := "https://doi6doi.github.io/vytools/";
      $purge := [$ixh,$rm,$inh,$ch,$ph,$dh,$th];
      links(true);
   }

   target {

      /// build documentation
      build {
         index();
         install();
         c();
         defs();
         cpp();
         tpl();
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
         if ( older( $rm, $ixx ))
            $Dox.build( $rm, $ixx );
         links(false);
         if ( older( $ixh, $ixx ))
            $Dox.build( $ixh, $ixx );
      }

      /// build Install.html
      install() {
         links(false);
         if ( older( $inh, $inx ))
            $Dox.build( $inh, $inx );
      }

      /// build C.html
      c() {
         links(false);
         if ( older( $ch, $cs ))
            $Dox.build( $ch, $cs );
      }

      /// build Defs.html
      defs() {
         links(false);
         if ( older( $dh, $ds ))
            $Dox.build( $dh, $ds );
      }

      /// build Cpp.html
      cpp() {
         links(false);
         if ( older( $ph, $ps ))
            $Dox.build( $ph, $ps );
      }

      /// build Templates.html
      tpl() {
         links(false);
         if ( older( $th, $ts ))
            $Dox.build( $th, $ts );
      }

      /// turn full links on or off
      links(x) {
         if (x)
            $Dox.set("linkHead",$base);
            else $Dox.set("linkHead",null);
      }

   }

}
