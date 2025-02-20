make {

   import { Git; }

   target {
      // download project
      download {
         Git.clone( "https://github.com/Doi6doi/vytools.git" );
      }
   }

}
