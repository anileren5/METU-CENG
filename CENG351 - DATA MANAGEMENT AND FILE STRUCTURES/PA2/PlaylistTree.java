import java.util.ArrayList;

public class PlaylistTree {
	
	public PlaylistNode primaryRoot;		//root of the primary B+ tree
	public PlaylistNode secondaryRoot;	//root of the secondary B+ tree
	public PlaylistTree(Integer order) {
		PlaylistNode.order = order;
		primaryRoot = new PlaylistNodePrimaryLeaf(null);
		primaryRoot.level = 0;
		secondaryRoot = new PlaylistNodeSecondaryLeaf(null);
		secondaryRoot.level = 0;
	}
	
	public void addSong(CengSong song) {
		// TODO: Implement this method to fill both primary and secondary tree

		/* ****************************************************************************** */
		/* 						INSERTION INTO PRIMARY TREE START						  */

		INSERT_INTO_PRIMARY_TREE(song);

		/* 						INSERTION INTO PRIMARY TREE END 						  */
		/* ****************************************************************************** */

		/* ****************************************************************************** */
		/* 						INSERTION INTO SECONDARY TREE START						  */

		INSERT_INTO_SECONDARY_TREE(song);

		/* 						INSERTION INTO SECONDARY TREE END 						  */
		/* ****************************************************************************** */
	}

	public CengSong searchSong(Integer audioId) {
		// TODO: Implement this method to find the song with the searched audioId in primary B+ tree and return value will not be tested, just print according to the specifications
		String TAB = "\t";
		ArrayList<PlaylistNode> path = new ArrayList<>();
		PlaylistNode tracker = primaryRoot;
		while (tracker.getType() != PlaylistNodeType.Leaf){
			path.add(tracker);
			int i = 0;
			while (i < ((PlaylistNodePrimaryIndex) tracker).audioIdCount() && audioId >= ((PlaylistNodePrimaryIndex) tracker).audioIdAtIndex(i)) i++;
			tracker = ((PlaylistNodePrimaryIndex) tracker).getChildrenAt(i);
		}
		CengSong song = null;
		for (int i=0;i<((PlaylistNodePrimaryLeaf) tracker).songCount();i++){
			if (((PlaylistNodePrimaryLeaf) tracker).songAtIndex(i).audioId() == audioId){
				song = ((PlaylistNodePrimaryLeaf) tracker).songAtIndex(i);
				break;
			}
		}

		for (int i=0;i<path.size();i++){
			String leveled_TAB = TAB.repeat(i);
			System.out.println(leveled_TAB + "<index>");
			for (int j=0;j<((PlaylistNodePrimaryIndex) path.get(i)).audioIdCount();j++) System.out.println(leveled_TAB + ((PlaylistNodePrimaryIndex) path.get(i)).audioIdAtIndex(j));
			System.out.println(leveled_TAB + "</index>");
		}

		if (song == null){
			System.out.println("Could not find " + audioId);
			return null;
		}
		else{
			String TAB_NEW = "\t".repeat(path.size());
			System.out.println(TAB_NEW+"<data>");
			System.out.println(TAB_NEW+"<record>"+song.fullName()+"</record>");
			System.out.println(TAB_NEW+"</data>");
			return song;
		}
	}
	public void printPrimaryPlaylist() {
		// TODO: Implement this method to print the primary B+ tree in Depth-first order
		printPrimaryHelper(primaryRoot, 0);
	}
	public void printSecondaryPlaylist() {
		// TODO: Implement this method to print the secondary B+ tree in Depth-first order
		printSecondaryHelper(secondaryRoot,0);
	}

	// Extra functions if needed

	private void printPrimaryHelper(PlaylistNode current_node, int current_level){
		String TAB = "\t".repeat(current_level);
		if(current_node.getType() == PlaylistNodeType.Leaf){
			System.out.println(TAB + "<data>");
			for(int i = 0; i < ((PlaylistNodePrimaryLeaf) current_node).songCount(); i++) System.out.println(TAB + "<record>" + ((PlaylistNodePrimaryLeaf) current_node).songAtIndex(i).fullName() + "</record>");
			System.out.println(TAB + "</data>");
		}
		else{
			System.out.println(TAB + "<index>");
			for(int i = 0; i < ((PlaylistNodePrimaryIndex) current_node).audioIdCount(); i++) System.out.println(TAB + ((PlaylistNodePrimaryIndex) current_node).audioIdAtIndex(i));
			System.out.println(TAB + "</index>");
			for(int i = 0; i <= ((PlaylistNodePrimaryIndex) current_node).audioIdCount(); i++) printPrimaryHelper(((PlaylistNodePrimaryIndex) current_node).getChildrenAt(i), current_level+1);
		}
	}
	private void printSecondaryHelper(PlaylistNode current_node, int current_level){
		String TAB = "\t".repeat(current_level);
		if (current_node.getType() == PlaylistNodeType.Leaf){
			System.out.println(TAB + "<data>");
			for (int i=0;i<((PlaylistNodeSecondaryLeaf)current_node).genreCount();i++){
				System.out.println(TAB + ((PlaylistNodeSecondaryLeaf)current_node).genreAtIndex(i));
				ArrayList<CengSong> current_bucket = ((PlaylistNodeSecondaryLeaf)current_node).getSongBucket().get(i);
				for (int j=0;j<current_bucket.size();j++) System.out.println("\t" + TAB  +  "<record>" + current_bucket.get(j).fullName() + "</record>");
			}
			System.out.println(TAB + "</data>");
		}
		else{
			System.out.println(TAB + "<index>");
			for(int i = 0; i < ((PlaylistNodeSecondaryIndex) current_node).genreCount(); i++) System.out.println(TAB + ((PlaylistNodeSecondaryIndex) current_node).genreAtIndex(i));
			System.out.println(TAB + "</index>");
			for(int i = 0; i <= ((PlaylistNodeSecondaryIndex) current_node).genreCount(); i++) printSecondaryHelper(((PlaylistNodeSecondaryIndex) current_node).getChildrenAt(i), current_level+1);
		}
	}
	private void INSERT_INTO_PRIMARY_TREE(CengSong song){
		int ORDER = PlaylistNode.order;
		int MAX = 2 * ORDER;
		int ID_TO_INSERT = song.audioId();
		// Inserting into primary tree:
		if (primaryRoot.getType() == PlaylistNodeType.Leaf) {
			// If the root node is lead i.e. there exists only one node in the tree which is root.
			int n_cur = ((PlaylistNodePrimaryLeaf) primaryRoot).songCount();
			if (n_cur < 2 * ORDER) {
				// There is enough space to insert an element.
				int i = 0;
				while (true) {
					int t = ((PlaylistNodePrimaryLeaf) primaryRoot).audioIdAtIndex(i);
					if (t == -1 || ID_TO_INSERT < t) {
						((PlaylistNodePrimaryLeaf) primaryRoot).addSong(i, song);
						break;
					}
					i++;
				}
			}
			else{ // The root (leaf) is full.
				ArrayList<Integer> audioIds = new ArrayList<>();
				ArrayList<CengSong> songs = new ArrayList<>();
				for (int i=0;i<2*ORDER;i++) {
					audioIds.add(((PlaylistNodePrimaryLeaf)primaryRoot).audioIdAtIndex(i));
					songs.add(((PlaylistNodePrimaryLeaf)primaryRoot).songAtIndex(i));
				}

				ArrayList<Integer> list1_ids = new ArrayList<>();
				ArrayList<Integer> list2_ids = new ArrayList<>();
				ArrayList<CengSong> list1_songs = new ArrayList<>();
				ArrayList<CengSong> list2_songs = new ArrayList<>();

				int n = 0;
				int m = -1;
				while (true){
					int t = audioIds.get(n);
					if (ID_TO_INSERT < t){
						m = n;
						break;
					}
					n++;
					if (n == audioIds.size()) break;
				}

				if (m == -1) {
					audioIds.add(ID_TO_INSERT);
					songs.add(song);
				}
				else {
					audioIds.add(m, ID_TO_INSERT);
					songs.add(m, song);
				}


				for (int i=0;i<ORDER;i++){
					list1_ids.add(audioIds.get(i));
					list1_songs.add(songs.get(i));
				}
				for (int i=ORDER;i<=2*ORDER;i++){
					list2_ids.add(audioIds.get(i));
					list2_songs.add(songs.get(i));
				}


				ArrayList<PlaylistNode> newRootChildren = new ArrayList<>();
				ArrayList<Integer> newRootKeys = new ArrayList<>();
				newRootKeys.add(list2_ids.get(0));
				PlaylistNodePrimaryIndex newRoot = new PlaylistNodePrimaryIndex(null, newRootKeys, newRootChildren);
				PlaylistNodePrimaryLeaf newLeaf1 = new PlaylistNodePrimaryLeaf(newRoot, list1_songs);
				PlaylistNodePrimaryLeaf newLeaf2 = new PlaylistNodePrimaryLeaf(newRoot, list2_songs);
				newRootChildren.add(newLeaf1);
				newRootChildren.add(newLeaf2);

				this.primaryRoot = newRoot;
			}
		}
		else{
			// The case there are multiple nodes.

			// Find the leaf node which is intented to be inserted.
			PlaylistNode suitableLeaf = ((PlaylistNodePrimaryIndex) primaryRoot).GetSuitableLeaf(ID_TO_INSERT);

			if (((PlaylistNodePrimaryLeaf)suitableLeaf).songCount() >= MAX){
				// The leaf is full.
				int i,j;
				PlaylistNode parent = suitableLeaf.getParent() ;
				i = 0;
				while (i != MAX && (song.audioId()) > ((PlaylistNodePrimaryLeaf)suitableLeaf).audioIdAtIndex(i)) i++;
				ArrayList<CengSong> list1 = new ArrayList<>();
				ArrayList<CengSong> list2 = new ArrayList<>();
				if(i < PlaylistNode.order ){
					for(j=0;j<ORDER-1;j++)list1.add(((PlaylistNodePrimaryLeaf)suitableLeaf).songAtIndex(j));
					list1.add(i ,song);
					for(;j!=MAX;j++)list2.add(((PlaylistNodePrimaryLeaf)suitableLeaf).songAtIndex(j)) ;
				}
				else {
					for(j=0;j<ORDER;j++) list1.add(((PlaylistNodePrimaryLeaf)suitableLeaf).songAtIndex(j)) ;
					for(;j!=MAX;j++) list2.add(((PlaylistNodePrimaryLeaf)suitableLeaf).songAtIndex(j)) ;
					list2.add( i-PlaylistNode.order , song) ;
				}
				PlaylistNodePrimaryLeaf newLeaf1 = new PlaylistNodePrimaryLeaf(null , list1);
				PlaylistNodePrimaryLeaf newLeaf2 = new PlaylistNodePrimaryLeaf(null , list2);
				if(((PlaylistNodePrimaryIndex)parent).audioIdCount() != MAX ){
					newLeaf1.setParent(parent) ; newLeaf2.setParent(parent) ;
					((PlaylistNodePrimaryIndex)parent).InsertIntoLeafLevelWithoutPush(newLeaf1,newLeaf2,list2.get(0).audioId()) ;
				}
				else{
					PlaylistNode temp = new PlaylistNode(null) ;
					temp = ((PlaylistNodePrimaryIndex)parent).PushUpOneLevelAtLeafLevel(parent, newLeaf1 , newLeaf2) ;
					if( temp != null) primaryRoot = temp ;
				}
			}
			else{
				// There is empty place in the leaf. Just insert into correct position.
				int i = 0;
				while (true) {
					int t = ((PlaylistNodePrimaryLeaf) suitableLeaf).audioIdAtIndex(i);
					if (t == -1 || ID_TO_INSERT < t) {
						((PlaylistNodePrimaryLeaf) suitableLeaf).addSong(i, song);
						break;
					}
					i++;
				}
			}
		}
	}
	private void INSERT_INTO_SECONDARY_TREE(CengSong song){
		int ORDER = PlaylistNode.order;
		int MAX = 2 * ORDER;
		String GENRE_TO_INSERT = song.genre();
		if (secondaryRoot.getType() == PlaylistNodeType.Leaf){
			// If the root node is leaf i.e. there exists only one node in the tree which is root.
			if (((PlaylistNodeSecondaryLeaf) secondaryRoot).genreCount() < MAX){
				// There is enough space to insert an element with different genre.
				// Check if the genre already exists.
				boolean flag = false;
				for (int i=0;i<((PlaylistNodeSecondaryLeaf) secondaryRoot).genreCount();i++){if (((PlaylistNodeSecondaryLeaf) secondaryRoot).genreAtIndex(i).compareTo(GENRE_TO_INSERT)  == 0){flag = true;break;}}
				if (!flag){ // Not already exists.
					int i=0;
					while (true){
						String t = ((PlaylistNodeSecondaryLeaf) secondaryRoot).genreAtIndex(i);
						if (t == null || GENRE_TO_INSERT.compareTo(t) < 0){
							((PlaylistNodeSecondaryLeaf) secondaryRoot).addSong(i,song);
							break;
						}
						i++;
					}
				}
				else {
					// Already Exists
					for (int i = 0; i < ((PlaylistNodeSecondaryLeaf) secondaryRoot).genreCount(); i++){
						if (((PlaylistNodeSecondaryLeaf) secondaryRoot).genreAtIndex(i).compareTo(GENRE_TO_INSERT)  == 0){
							((PlaylistNodeSecondaryLeaf) secondaryRoot).addSong(i,song);
						}
					}
				}
			}
			else{
				// The root (leaf) is full.
				// There is not enough space to insert an element with different genre but we can insert an element with existing genre.
				// Check if the genre already exists.
				boolean flag = false;
				for (int i=0;i<((PlaylistNodeSecondaryLeaf) secondaryRoot).genreCount();i++){if (((PlaylistNodeSecondaryLeaf) secondaryRoot).genreAtIndex(i).compareTo(GENRE_TO_INSERT)  == 0){flag = true;break;}}
				if (flag){ // Existing genre
					for (int i = 0; i < ((PlaylistNodeSecondaryLeaf) secondaryRoot).genreCount(); i++){
						if (((PlaylistNodeSecondaryLeaf) secondaryRoot).genreAtIndex(i).compareTo(GENRE_TO_INSERT)  == 0){
							((PlaylistNodeSecondaryLeaf) secondaryRoot).addSong(i,song);
						}
					}
				}
				else{ // Not-existing genre
					ArrayList<String> genres = new ArrayList<>();
					ArrayList<ArrayList<CengSong>> bucketLists = new ArrayList<>();
					for (int i=0;i<MAX;i++){
						genres.add(((PlaylistNodeSecondaryLeaf) secondaryRoot).genreAtIndex(i));
						bucketLists.add(((PlaylistNodeSecondaryLeaf) secondaryRoot).getSongBucket().get(i));
					}

					ArrayList<String> list1_genres = new ArrayList<>();
					ArrayList<String> list2_genres = new ArrayList<>();
					ArrayList<ArrayList<CengSong>> list1_songs = new ArrayList<>();
					ArrayList<ArrayList<CengSong>> list2_songs = new ArrayList<>();

					int n=0;
					int m=-1;
					while(true){
						String t = genres.get(n);
						if (GENRE_TO_INSERT.compareTo(t) < 0){
							m = n;
							break;
						}
						n++;
						if (n == genres.size()) break;
					}

					if (m==-1){
						genres.add(GENRE_TO_INSERT);
						ArrayList<CengSong> newBucketList = new ArrayList<>();
						newBucketList.add(song);
						bucketLists.add(newBucketList);
					}
					else{
						genres.add(m, GENRE_TO_INSERT);
						ArrayList<CengSong> newBucketList = new ArrayList<>();
						newBucketList.add(song);
						bucketLists.add(m,newBucketList);
					}

					for (int i=0;i<ORDER;i++){
						list1_genres.add(genres.get(i));
						list1_songs.add(bucketLists.get(i));
					}
					for (int i=ORDER;i<=MAX;i++){
						list2_genres.add(genres.get(i));
						list2_songs.add(bucketLists.get(i));
					}

					ArrayList<PlaylistNode> newRootChildren = new ArrayList<>();
					ArrayList<String> newRootGenres = new ArrayList<>();
					newRootGenres.add(list2_genres.get(0));
					PlaylistNodeSecondaryIndex newRoot = new PlaylistNodeSecondaryIndex(null, newRootGenres, newRootChildren);
					PlaylistNodeSecondaryLeaf newLeaf1 = new PlaylistNodeSecondaryLeaf(newRoot, list1_songs);
					PlaylistNodeSecondaryLeaf newLeaf2 = new PlaylistNodeSecondaryLeaf(newRoot, list2_songs);
					newRootChildren.add(newLeaf1);
					newRootChildren.add(newLeaf2);

					this.secondaryRoot = newRoot;
				}
			}
		}
		else{
			// The case there are multiple nodes in the secondary tree.

			/*                  		OUTLINE                                       */
			// First find the leaf node where the song is wanted to be inserted.
			// Second check if the same genre exists.
			// If the same genre exists, just insert it there.
			// Otherwise, check if there is an empty slot.
			// If so, just insert into the empty slot.
			// Otherwise, PUSH
			/*                 			OUTLINE END                                     */

			// First find the leaf node where the song is wanted to be inserted.
			PlaylistNode suitableLeaf = ((PlaylistNodeSecondaryIndex)secondaryRoot).GetSuitableLeaf(GENRE_TO_INSERT);

			// Second check if the same genre exists.
			boolean flag = false;
			for (int i=0;i<((PlaylistNodeSecondaryLeaf) suitableLeaf).genreCount();i++){if (((PlaylistNodeSecondaryLeaf) suitableLeaf).genreAtIndex(i).compareTo(GENRE_TO_INSERT)  == 0){flag = true;break;}}

			if (flag){// If the same genre exists, just insert it there.
				for (int i = 0; i < ((PlaylistNodeSecondaryLeaf) suitableLeaf).genreCount(); i++){
					if (((PlaylistNodeSecondaryLeaf) suitableLeaf).genreAtIndex(i).compareTo(GENRE_TO_INSERT)  == 0){
						((PlaylistNodeSecondaryLeaf) suitableLeaf).addSong(i,song);
					}
				}
			}
			else{ // Otherwise, check if there is an empty slot.
				if (((PlaylistNodeSecondaryLeaf)suitableLeaf).genreCount() < MAX){
					// There is an empty slot, just insert into empty slot.
					int i=0;
					while (true){
						String t = ((PlaylistNodeSecondaryLeaf)suitableLeaf).genreAtIndex(i);
						if (t == null || t.compareTo("Not Valid Index!!!") == 0 || GENRE_TO_INSERT.compareTo(t) < 0){
							ArrayList<CengSong> temp = new ArrayList<>();
							temp.add(song);
							((PlaylistNodeSecondaryLeaf) suitableLeaf).getSongBucket().add(i,temp);
							break;
						}
						i++;
					}
				}
				else {
					// THERE IS NOT EMPTY SLOT: THIS IS THE HARD PARD !!!
					/* --------------------------------------- CORE PART START ------------------------------------------ */
					int i,j;
					PlaylistNode parent = suitableLeaf.getParent();
					i=0;
					while (i != MAX && GENRE_TO_INSERT.compareTo(((PlaylistNodeSecondaryLeaf) suitableLeaf).genreAtIndex(i)) > 0) i++;
					ArrayList<ArrayList<CengSong>> list1 = new ArrayList<>();
					ArrayList<ArrayList<CengSong>> list2 = new ArrayList<>();
					if (i<ORDER){
						for (j=0;j<ORDER-1;j++) list1.add(((PlaylistNodeSecondaryLeaf) suitableLeaf).getSongBucket().get(j));
						ArrayList<CengSong> temp = new ArrayList<>();
						temp.add(song);
						list1.add(i,temp);
						for (;j!=MAX;j++) list2.add(((PlaylistNodeSecondaryLeaf) suitableLeaf).getSongBucket().get(j));
					}
					else {
						for (j=0;j<ORDER;j++) list1.add(((PlaylistNodeSecondaryLeaf) suitableLeaf).getSongBucket().get(j));
						for (;j!=MAX;j++) list2.add(((PlaylistNodeSecondaryLeaf) suitableLeaf).getSongBucket().get(j));
						ArrayList<CengSong> temp = new ArrayList<>();
						temp.add(song);
						list2.add(i-ORDER, temp);
					}
					PlaylistNodeSecondaryLeaf newLeaf1 = new PlaylistNodeSecondaryLeaf(null, list1);
					PlaylistNodeSecondaryLeaf newLeaf2 = new PlaylistNodeSecondaryLeaf(null, list2);
					if (((PlaylistNodeSecondaryIndex)parent).genreCount() != MAX){
						newLeaf1.setParent(parent);
						newLeaf2.setParent(parent);
						((PlaylistNodeSecondaryIndex)parent).InsertIntoLeafLevelWithoutPush(newLeaf1, newLeaf2, list2.get(0).get(0).genre());
					}
					else{
						PlaylistNode temp = new PlaylistNode(null);
						temp = ((PlaylistNodeSecondaryIndex)parent).PushUpOneLevelAtLeafLevel(parent, newLeaf1, newLeaf2);
						if (temp != null) secondaryRoot = temp;
					}
					/* --------------------------------------- CORE PART END -------------------------------------------- */
				}
			}
		}
	}

}


