make {

   import { Git; }

   target {
      // download project
      download() {
         Git.clone( "Doi6doi/vytools" );
      }
   }

}
