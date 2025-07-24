make {

   init {
      $name := "vytools";
      $gitUrl := "https://github.com/Doi6doi/vytools.git";
      $author := "Várnagy Zoltán";
      $ver := "20250724";
      
      $C := tool( "C", { libMode:true, show:true });
      $Cpp := tool( "Cpp", { libMode:true, show:true });
      $Git := tool( "Git" );
      
      $cdep := "c.dep";
      $pdep := "p.dep";
      $cs := ["vytools.c"];
      $hs := ["vytools_defs.h","vytools.h"];
      $his := ["vytools_impl.h"];
      $hps := ["vytools.hpp","vytools_tpl.hpp"];
      $cps := ["vytoolspp.cpp","vyt_str.cpp"];
      case ( system() ) {
         "Linux": $cs += "linux.c";
         "Windows": $cs += "windows.c";
      }
      $buildDir := "build";
      $ccs := regexp( $cs, "#(.*)\\.c#", "p_\\1.cpp" );
      $clib := $C.libFile( $name );
      $plib := $Cpp.libFile( $name+"p" );
      $purge := ["*.o","*.so","*.lib","*.dll","*.dep",$buildDir] + $ccs;
   }

   target {

      /// run menu
      menu {
         $Dlg := tool( "Dialog" );
         m := $Dlg.menu("vyTools")
            .item("VyTools is a C and C++ library which"
            +" makes often used tasks easier.");
         if ( inDir() ) {
            m.item("Build libraries",build)
             .item("Clean generated files",clean)
             .item("Generate documentation",docs)
             .item("Run tests",test);
            case (system()) {
               "Linux": m.item("Create Deb package",deb);
               "Windows": m.item("Create Windows zip", wzip );
            }
         } else {
            m.item("Download source",download);
         }
         m.exec();
      }

      /// download the project
      download {
         $Git.clone( $gitUrl );
         cd("vytools");
      }

     /// build project
      build {
         genCcs();
         genDep();
         genObjs();
         genLibs();
      }

      /// clear generated files
      clean {
        purge( $purge );
      }

      /// create documentation
      docs {
         make("docs");
      }

      /// create Debian deb package
      deb {
         build();
         makeDeb();
      }

      wzip {
         build();
         makeWZip();
      }

      /// run tests
      test {
         make("test");
      }

   }

   function {
      /// are we in project directory
      inDir() {
         return exists("vytools.h");
      }

      /// generate copy of .c files for c++ compiling
      genCcs() {
         foreach ( c | $cs ) {
            cc := regexp( c, "#(.*)\\.c#", "p_\\1.cpp" );
            if ( older( cc, c ))
               copy( c, cc );
         }
      }

      /// generate dependency files
      genDep() {
         if ( older( $cdep, $cs+$hs+$his ) )
            $C.depend( $cdep, $cs );
         if ( older( $pdep, $ccs+$cps+$hs+$his+$hps ) )
            $Cpp.depend( $pdep, $ccs+$cps );
      }

      /// generate object files
      genObjs() {
         ds := $C.loadDep( $cdep );
         foreach ( c | $cs ) {
            o := changeExt( c, $C.objExt() );
            if ( older( o, ds[o] ))
               $C.compile( o, c );
         }
         ds := $Cpp.loadDep( $pdep );
         foreach ( c | $ccs + $cps ) {
            o := changeExt( c, $Cpp.objExt() );
            if ( older( o, ds[o] ))
               $Cpp.compile( o, c );
         }
      }

      /// generate library
      genLibs() {
         os := changeExt( $cs, $C.objExt() );
         if ( older( $clib, os ))
            $C.link( $clib, os );
         pos := changeExt( $cps+$ccs, $Cpp.objExt() );
         if ( older( $plib, pos ))
            $Cpp.link( $plib, pos );
      }

      makeDeb() {
         Deb := tool("Deb");
         Dox := tool("Dox");
         mkdir( $buildDir );
         // copy libs
         blDir := path( $buildDir, "usr/lib" );
         mkdir( blDir );
         foreach ( l | [$clib,$plib] )
            copy( l, path( blDir, l ));
         // copy headers
         biDir := path( $buildDir, "usr/include" );
         mkdir( biDir );
         foreach ( h | $hs+$hps )
            copy( h, path( biDir, h ));
         // create description
         bdDir := path( $buildDir, "DEBIAN" );
         mkdir( bdDir );
         Dox.read("docs/desc.dox");
         Dox.set("outType","txt");
         ds := replace(Dox.write(), "\n", "\n " );
         // create DEBIAN/control
         cnt := [
            "Package: "+$name,
            "Version: "+$ver,
            "Architecture: "+Deb.arch( arch() ),
            "Maintainer: "+$author+" <"+$gitUrl+">",
            "Description: "+ds
         ];
         saveFile( path( bdDir, "control" ), implode("\n",cnt) );
         // build package
         Deb.build( $buildDir );
      }

      makeWZip() {
         Zip := tool("Zip");
         zf := $name+"_"+$ver+"_win.zip";
         fs := [$clib,$plib]+$hs+$hps;
         foreach (l : [$clib,$plib]) {
            l := changeExt(l,".lib");
            if ( exists(l))
               fs += l;
         }
         Zip.pack( zf, fs );
      }


   }

}
