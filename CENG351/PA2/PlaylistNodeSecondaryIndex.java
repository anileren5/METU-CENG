import java.util.ArrayList;

public class PlaylistNodeSecondaryIndex extends PlaylistNode {
	private ArrayList<String> genres;
	private ArrayList<PlaylistNode> children;

	public PlaylistNodeSecondaryIndex(PlaylistNode parent) {
		super(parent);
		genres = new ArrayList<String>();
		children = new ArrayList<PlaylistNode>();
		this.type = PlaylistNodeType.Internal;
	}
	
	public PlaylistNodeSecondaryIndex(PlaylistNode parent, ArrayList<String> genres, ArrayList<PlaylistNode> children) {
		super(parent);
		this.genres = genres;
		this.children = children;
		this.type = PlaylistNodeType.Internal;
	}
	
	// GUI Methods - Do not modify
	public ArrayList<PlaylistNode> getAllChildren()
	{
		return this.children;
	}
	
	public PlaylistNode getChildrenAt(Integer index) {
		
		return this.children.get(index);
	}
	

	public Integer genreCount()
	{
		return this.genres.size();
	}
	
	public String genreAtIndex(Integer index) {
		if(index >= this.genreCount() || index < 0) {
			return "Not Valid Index!!!";
		}
		else {
			return this.genres.get(index);
		}
	}
	
	
	// Extra functions if needed

	public PlaylistNode PushUpOneLevelAtLeafLevel(PlaylistNode parent, PlaylistNodeSecondaryLeaf newLeaf1 , PlaylistNodeSecondaryLeaf newLeaf2){
		int ORDER = PlaylistNode.order;
		int MAX = 2 * ORDER;

		String genreToBePushed;

		ArrayList<PlaylistNode> children1 = new ArrayList<>();
		ArrayList<PlaylistNode> children2 = new ArrayList<>();
		ArrayList<String> genres1 = new ArrayList<>();
		ArrayList<String> genres2 = new ArrayList<>();

		int i=0;

		while (i<MAX){
			if (newLeaf2.genreAtIndex(0).compareTo(((PlaylistNodeSecondaryIndex)parent).genreAtIndex(i)) > 0){
				i++ ;
				continue;
			}
			break;
		}

		if(i<ORDER) {
			int j=0;
			while (j<ORDER-1){genres1.add(((PlaylistNodeSecondaryIndex)parent).genreAtIndex(j));j++;}
			genres1.add(i,newLeaf2.getSongBucket().get(0).get(0).genre());
			while (j<MAX){genres2.add(((PlaylistNodeSecondaryIndex)parent).genreAtIndex(j));j++;}
		}
		else{
			int j=0;
			while (j<ORDER){genres1.add(((PlaylistNodeSecondaryIndex)parent).genreAtIndex(j));j++;}
			while (j<MAX){genres2.add(((PlaylistNodeSecondaryIndex)parent).genreAtIndex(j));j++;}
			genres2.add(i-ORDER , newLeaf2.genreAtIndex(0) ) ;
		}

		genreToBePushed = genres2.get(0) ;
		genres2.remove(0) ;

		if(i<ORDER) {
			int j=0;
			while (j<ORDER){children1.add(j, children.get(j));j++;}
			children1.remove(i);
			children1.add(i, newLeaf1 );
			children1.add(i+1, newLeaf2);
			while (j<=MAX){children2.add(j-ORDER, children.get(j));j++;}
		}
		else if(i == ORDER) {
			int j=0;
			while (j<ORDER){children1.add(j, children.get(j));j++;}
			children1.add(j,newLeaf1) ;
			children2.add(0 , newLeaf2 ) ;
			j++ ;
			while (j<=MAX){children2.add(j - ORDER, this.children.get(j));j++;}
		}
		else if (i==2*MAX){
			int j=0;
			while (j<ORDER){children1.add(j, children.get(j));j++;}
			children1.add(j,newLeaf1) ;
			children2.add(0 , newLeaf2 ) ;
			j++ ;
			while (j<=MAX){children2.add(j - ORDER, this.children.get(j));j++;}
			j=0;
			while (j<=MAX){children2.add(j - ORDER, this.children.get(j));j++;}
		}
		else if (i > MAX){
			int j=0;
			while (j<ORDER){children1.add(j, children.get(j));j++;}
			children1.add(j,newLeaf1) ;
			children2.add(0 , newLeaf2 ) ;
			j++ ;
			while (j>MAX){children2.add(j - ORDER, this.children.remove(j));j--;}
		}
		else{
			int j = 0;
			while (j<=ORDER){children1.add(j, children.get(j));j++;}
			while (j<=MAX){children2.add(j-ORDER-1, this.children.get(j));j++;}
			children2.remove(i-ORDER-1);
			children2.add(i-ORDER-1, newLeaf1);
			children2.add(i-ORDER, newLeaf2);
		}

		if(parent.getParent() == null) {
			ArrayList<String> newRootGenres = new ArrayList<>();
			newRootGenres.add(genreToBePushed);
			ArrayList<PlaylistNode> newRootChildren = new ArrayList<>();
			PlaylistNodeSecondaryIndex leftSplit = new PlaylistNodeSecondaryIndex( null, genres1, children1);
			PlaylistNodeSecondaryIndex rightSplit = new PlaylistNodeSecondaryIndex( null, genres2, children2);
			newRootChildren.add(leftSplit);
			newRootChildren.add(rightSplit);
			PlaylistNodeSecondaryIndex newRoot = new PlaylistNodeSecondaryIndex( null, newRootGenres, newRootChildren);
			leftSplit.setParent(newRoot);
			rightSplit.setParent(newRoot);
			int j = 0;
			while (j<children1.size()){(children1.get(j)).setParent(leftSplit);j++;}
			j = 0;
			while (j<children2.size()){(children2.get(j)).setParent(rightSplit);j++;}
			return newRoot;
		}

		else{
			PlaylistNodeSecondaryIndex parent2 = ((PlaylistNodeSecondaryIndex)(parent.getParent()));
			PlaylistNodeSecondaryIndex leftSplit = new PlaylistNodeSecondaryIndex( null, genres1, children1) ;
			PlaylistNodeSecondaryIndex rightSplit = new PlaylistNodeSecondaryIndex( null, genres2, children2) ;

			int j=0;
			while (j<children1.size()){(children1.get(j)).setParent(leftSplit);j++;}
			j = 0;
			while (j<children2.size()){(children2.get(j)).setParent(rightSplit);j++;}

			if(parent2.genreCount() != MAX){
				j=0;
				while (j<parent2.genreCount()){
					if (genreToBePushed.compareTo(parent2.genreAtIndex(j))>0) j++;
					 else break;
				}
				parent2.children.remove(j);
				parent2.genres.add(j, genreToBePushed);
				parent2.children.add(j, leftSplit);
				parent2.children.add(j+1, rightSplit);

				leftSplit.setParent(parent2);
				rightSplit.setParent(parent2);
				return null ;
			}
			else return PushUpHigherLevels(genreToBePushed, leftSplit,rightSplit,parent2);
		}
	}
	public PlaylistNode PushUpHigherLevels(String genreToBePushedUp, PlaylistNodeSecondaryIndex leftSplit , PlaylistNodeSecondaryIndex rightSplit , PlaylistNodeSecondaryIndex parent){
		int ORDER = PlaylistNode.order;
		int MAX = 2 * ORDER;

		ArrayList<String> genres1 = new ArrayList<>();
		ArrayList<String> genres2 = new ArrayList<>();
		ArrayList<PlaylistNode> children1 = new ArrayList<>();
		ArrayList<PlaylistNode> children2 = new ArrayList<>();

		while(true){
			int i = 0;
			while (i<parent.genreCount()){if (genreToBePushedUp.compareTo(parent.genreAtIndex(i))>0)i++;else break;}
			if(i<ORDER) {
				int j=0;
				while (j<ORDER-1){genres1.add(j, parent.genreAtIndex(j));j++;}
				while (j<MAX){genres2.add(j-ORDER+1, parent.genreAtIndex(j));j++;}
				genres1.add(i, genreToBePushedUp );
			}

			else{
				int j = 0;
				while (j < MAX) {
					if (j<ORDER) genres1.add(j, parent.genreAtIndex(j));
					else genres2.add(j-ORDER, parent.genreAtIndex(j));
					j++;
				}
				genres2.add(i-ORDER , genreToBePushedUp );
			}

			if(i<ORDER){
				int j = 0;
				while (j <= MAX) {
					if (j < ORDER) children1.add(j, parent.getChildrenAt(j));
					else children2.add(j-ORDER, parent.getChildrenAt(j));
					j++;
				}
				children1.remove(i) ;
				children1.add(i, leftSplit);
				children1.add(i+1, rightSplit);
			}

			else if (i == ORDER){
				int j=0;
				for(j=0;j < ORDER; j++) children1.add(j , parent.getChildrenAt(j));
				children1.add(j, leftSplit);
				j+=1;
				children2.add(0, rightSplit);
				for(;j<=MAX;j++)children2.add(j - ORDER , parent.getChildrenAt(j));
			}

			else{
				int j=0;
				while (j<=MAX){
					if (j <= ORDER) children1.add(j, parent.getChildrenAt(j));
					else children2.add(j-ORDER-1, parent.getChildrenAt(j));
					j++;
				}
				children2.remove(i-ORDER-1);
				children2.add(i-ORDER-1, leftSplit);
				children2.add(i-ORDER, rightSplit);
			}


			if( ((PlaylistNodeSecondaryIndex)(parent.getParent())) == null ) {
				ArrayList<String> newUpperRootGenres = new ArrayList<>();
				genreToBePushedUp = genres2.get(0);
				genres2.remove(0);
				newUpperRootGenres.add(genreToBePushedUp);
				ArrayList<PlaylistNode> newUpperRootChildren = new ArrayList<>();
				PlaylistNodeSecondaryIndex upperLeftSplit = new PlaylistNodeSecondaryIndex( null , genres1, children1);
				PlaylistNodeSecondaryIndex upperRightSplit = new PlaylistNodeSecondaryIndex( null , genres2, children2);
				newUpperRootChildren.add(upperLeftSplit) ; newUpperRootChildren.add(upperRightSplit);
				PlaylistNodeSecondaryIndex newHigherPushedRoot = new PlaylistNodeSecondaryIndex( null, newUpperRootGenres, newUpperRootChildren);
				upperRightSplit.setParent(newHigherPushedRoot);
				upperLeftSplit.setParent(newHigherPushedRoot);
				int j; for(j=0;j<children1.size();j++) (children1.get(j)).setParent(upperLeftSplit);
				for(j=0;j<children2.size();j++)(children2.get(j)).setParent(upperRightSplit);
				return newHigherPushedRoot;
			}

			else if(((PlaylistNodeSecondaryIndex)(parent.getParent())).genreCount() < MAX){
				genreToBePushedUp = genres2.get(0);
				genres2.remove(0) ;
				PlaylistNodeSecondaryIndex UpperLeftSplit = new PlaylistNodeSecondaryIndex( null , genres1 , children1) ;
				PlaylistNodeSecondaryIndex UpperRightSplit = new PlaylistNodeSecondaryIndex( null , genres2 , children2) ;
				int j;
				for(j=0;j<children1.size();j++)(children1.get(j)).setParent(UpperLeftSplit);
				for(j=0;j<children2.size();j++)(children2.get(j)).setParent(UpperRightSplit);
				for(j = 0 ; j < ((PlaylistNodeSecondaryIndex)(parent.getParent())).genreCount();j++){
					if (genreToBePushedUp.compareTo(((PlaylistNodeSecondaryIndex)(parent.getParent())).genreAtIndex(j))>0);
					else break;
				}
				((PlaylistNodeSecondaryIndex)(parent.getParent())).children.remove(j);
				((PlaylistNodeSecondaryIndex)(parent.getParent())).genres.add(j,genreToBePushedUp);
				((PlaylistNodeSecondaryIndex)(parent.getParent())).children.add(j,UpperLeftSplit);
				((PlaylistNodeSecondaryIndex)(parent.getParent())).children.add(j+1,UpperRightSplit);
				UpperLeftSplit.setParent(((PlaylistNodeSecondaryIndex)(parent.getParent())));
				UpperRightSplit.setParent(((PlaylistNodeSecondaryIndex)(parent.getParent())));
				return null;
			}

			else{
				genreToBePushedUp = genres2.get(0);
				genres2.remove(0);
				PlaylistNodeSecondaryIndex UpperLeftSplit = new PlaylistNodeSecondaryIndex( null, genres1, children1);
				PlaylistNodeSecondaryIndex UpperHigherSplit = new PlaylistNodeSecondaryIndex( null, genres2, children2);

				int j = 0;
				while (j<children1.size()){children1.get(j).setParent(UpperLeftSplit);j++;}
				j = 0;
				while (j<children2.size()){children2.get(j).setParent(UpperHigherSplit);j++;}
				return PushUpHigherLevels(genreToBePushedUp,UpperLeftSplit,UpperHigherSplit,((PlaylistNodeSecondaryIndex)(parent.getParent()))) ;
			}
		}
	}
	public void InsertIntoLeafLevelWithoutPush(PlaylistNodeSecondaryLeaf newLeaf1, PlaylistNodeSecondaryLeaf newLeaf2, String genreToBePushed){
		int index = 0;
		while (index < genres.size() && genres.get(index).compareTo(genreToBePushed) < 0) index++;
		genres.add(index, genreToBePushed);
		children.remove(index);
		children.add(index, newLeaf1);
		children.add(index+1, newLeaf2);
	}

	public PlaylistNode GetSuitableLeaf(String GENRE_TO_INSERT){
		PlaylistNode tracker = this;
		while (tracker.getType() != PlaylistNodeType.Leaf){
			int i=0;
			while (true){
				if (((PlaylistNodeSecondaryIndex)tracker).genreAtIndex(i).compareTo("Not Valid Index!!!") == 0) break;
				if (GENRE_TO_INSERT.compareTo(((PlaylistNodeSecondaryIndex)tracker).genreAtIndex(i)) < 0) break;
				else i++;
			}
			tracker = ((PlaylistNodeSecondaryIndex)tracker).getChildrenAt(i);
		}
		return tracker;
	}
}
