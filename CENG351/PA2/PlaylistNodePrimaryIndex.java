import java.util.ArrayList;

public class PlaylistNodePrimaryIndex extends PlaylistNode {
	private ArrayList<Integer> audioIds;
	private ArrayList<PlaylistNode> children;
	
	public PlaylistNodePrimaryIndex(PlaylistNode parent) {
		super(parent);
		audioIds = new ArrayList<Integer>();
		children = new ArrayList<PlaylistNode>();
		this.type = PlaylistNodeType.Internal;
	}
	
	public PlaylistNodePrimaryIndex(PlaylistNode parent, ArrayList<Integer> audioIds, ArrayList<PlaylistNode> children) {
		super(parent);
		this.audioIds = audioIds;
		this.children = children;
		this.type = PlaylistNodeType.Internal;
	}
	
	// GUI Methods - Do not modify
	public ArrayList<PlaylistNode> getAllChildren()
	{
		return this.children;
	}
	
	public PlaylistNode getChildrenAt(Integer index) {return this.children.get(index); }
	
	public Integer audioIdCount()
	{
		return this.audioIds.size();
	}
	public Integer audioIdAtIndex(Integer index) {
		if(index >= this.audioIdCount() || index < 0) {
			return -1;
		}
		else {
			return this.audioIds.get(index);
		}
	}
	
	// Extra functions if needed
	public PlaylistNode PushUpHigherLevels(Integer audioIdToBePushedUp, PlaylistNodePrimaryIndex leftSplit , PlaylistNodePrimaryIndex rightSplit , PlaylistNodePrimaryIndex parent) {
		int ORDER = PlaylistNode.order;
		int MAX = 2 * ORDER;
		int i,j;

		ArrayList<Integer> audioIds1 = new ArrayList<>();
		ArrayList<Integer> audioIds2 = new ArrayList<>();

		while(true){ // Push until you find an empty slot in the parent node.
			for(i=0;i<parent.audioIdCount();i++){
				if(audioIdToBePushedUp > parent.audioIdAtIndex(i));
				else break ;
			}
			if(i>=ORDER){
				j = 0;
				while (j<ORDER){audioIds1.add(j, parent.audioIdAtIndex(j));j++;}
				while (j<MAX){audioIds2.add(j-ORDER, parent.audioIdAtIndex(j));j++;}
				audioIds2.add(i-ORDER, audioIdToBePushedUp );
			}
			else{
				for(j=0;j<ORDER-1;j++) audioIds1.add(j, parent.audioIdAtIndex(j));
				for(;j<MAX;j++)audioIds2.add(j-ORDER +1, parent.audioIdAtIndex(j));
				audioIds1.add(i,audioIdToBePushedUp);
			}
			ArrayList<PlaylistNode> children1 = new ArrayList<>();
			ArrayList<PlaylistNode> children2 = new ArrayList<>();
			if(i<ORDER){
				j=0;
				while (j < ORDER) {
					children1.add(j, parent.getChildrenAt(j));
					j++;
				}
				while (j <= MAX) {
					children2.add(j-ORDER, parent.getChildrenAt(j));
					j++;
				}
				children1.remove(i);
				children1.add(i , leftSplit);
				children1.add(i+1 , rightSplit);
			}
			else if (i > ORDER){
				for(j=0;j<=ORDER;j++) children1.add(j, parent.getChildrenAt(j));
				for(;j<=MAX;j++)children2.add(j-ORDER-1, parent.getChildrenAt(j));
				children2.remove(i-ORDER-1);
				children2.add(i-ORDER-1, leftSplit);
				children2.add(i-ORDER, rightSplit);
			}
			else{
				for(j=0;j<ORDER;j++) children1.add(j, parent.getChildrenAt(j));
				children1.add(j , leftSplit);
				j++;
				children2.add(0 , rightSplit);
				for(;j<=MAX;j++) children2.add(j-ORDER , parent.getChildrenAt(j) );
			}
			PlaylistNodePrimaryIndex parent2 = ( ( PlaylistNodePrimaryIndex ) (parent.getParent() ) ) ;

			if(parent2 == null) {
				ArrayList<Integer> UpperNewRootAudioIds = new ArrayList<>();
				audioIdToBePushedUp = audioIds2.get(0);
				audioIds2.remove(0);
				UpperNewRootAudioIds.add(audioIdToBePushedUp);
				ArrayList<PlaylistNode> UpperNewRootChildren = new ArrayList<>();
				PlaylistNodePrimaryIndex UpperLeftSplit = new PlaylistNodePrimaryIndex( null, audioIds1, children1);
				PlaylistNodePrimaryIndex UpperRightSplit = new PlaylistNodePrimaryIndex( null, audioIds2, children2);
				UpperNewRootChildren.add(UpperLeftSplit);
				UpperNewRootChildren.add(UpperRightSplit);
				PlaylistNodePrimaryIndex UpperNewRoot = new PlaylistNodePrimaryIndex( null, UpperNewRootAudioIds, UpperNewRootChildren) ;
				UpperLeftSplit.setParent(UpperNewRoot);
				UpperRightSplit.setParent(UpperNewRoot);
				for (PlaylistNode children: children1) children.setParent(UpperLeftSplit);
				for (PlaylistNode children: children2) children.setParent(UpperRightSplit);
				return UpperNewRoot;
			}
			else if(parent2.audioIdCount() != MAX){
				audioIdToBePushedUp = audioIds2.get(0);
				audioIds2.remove(0);
				PlaylistNodePrimaryIndex UpperLeftSplit = new PlaylistNodePrimaryIndex( null, audioIds1, children1);
				PlaylistNodePrimaryIndex UpperRightSplit = new PlaylistNodePrimaryIndex( null, audioIds2, children2);
				for (PlaylistNode children: children1) children.setParent(UpperLeftSplit);
				for (PlaylistNode children: children2) children.setParent(UpperRightSplit);
				for(j=0; j<parent2.audioIdCount(); j++){
					if(audioIdToBePushedUp > parent2.audioIdAtIndex(j)) continue;
					else break ;
				}
				parent2.children.remove(j);
				parent2.audioIds.add(j, audioIdToBePushedUp);
				parent2.children.add(j, UpperLeftSplit);
				parent2.children.add(j+1, UpperRightSplit);
				UpperLeftSplit.setParent(parent2);
				UpperRightSplit.setParent(parent2);
				return null;
			}
			else if (parent2.audioIdCount() > 2*MAX){ // Check Boundary Errors
				audioIdToBePushedUp = audioIds2.get(0);
				audioIds2.remove(0);
				PlaylistNodePrimaryIndex UpperLeftSplit = new PlaylistNodePrimaryIndex( null, audioIds1, children1);
				PlaylistNodePrimaryIndex UpperRightSplit = new PlaylistNodePrimaryIndex( null, audioIds2, children2);
				for(j=0; j<parent2.audioIdCount(); j++){
					if(audioIdToBePushedUp > parent2.audioIdAtIndex(j)) continue;
					else break ;
				}
				parent2.children.remove(j);
				parent2.children.add(j+1, UpperRightSplit);
				UpperLeftSplit.setParent(parent2);
				UpperRightSplit.setParent(parent2);
				return null;
			}
			else if (parent2.audioIdCount() > 2*ORDER + 1){ // Check Boundary Errors
				PlaylistNodePrimaryIndex UpperLeftSplit = new PlaylistNodePrimaryIndex( null, audioIds1, children1);
				PlaylistNodePrimaryIndex UpperRightSplit = new PlaylistNodePrimaryIndex( null, audioIds2, children2);
				parent2.children.remove(j);
				parent2.children.add(j+1, UpperRightSplit);
				UpperLeftSplit.setParent(parent2);
				UpperRightSplit.setParent(parent2);
			}
			else{
				audioIdToBePushedUp = audioIds2.get(0);
				audioIds2.remove(0);
				PlaylistNodePrimaryIndex UpperLeftSplit = new PlaylistNodePrimaryIndex( null, audioIds1, children1);
				PlaylistNodePrimaryIndex UpperRightSplit = new PlaylistNodePrimaryIndex( null, audioIds2, children2);
				for (PlaylistNode children: children1) children.setParent(UpperLeftSplit);
				for (PlaylistNode children: children2) children.setParent(UpperRightSplit);
				return PushUpHigherLevels(audioIdToBePushedUp, UpperLeftSplit, UpperRightSplit, parent2);
			}
		}
	}
	public void InsertIntoLeafLevelWithoutPush(PlaylistNodePrimaryLeaf newLeaf1, PlaylistNodePrimaryLeaf newLeaf2, Integer audioIdToBePushed) {
		// Find the index at which the new leaf node should be inserted
		int index = 0;
		while (index < audioIds.size() && audioIds.get(index) < audioIdToBePushed) {
			index++;
		}
		// Insert the new leaf node and update the keys of the index node
		audioIds.add(index, audioIdToBePushed);
		children.remove(index);
		children.add(index, newLeaf1);
		children.add(index + 1, newLeaf2);
	}
	public PlaylistNode PushUpOneLevelAtLeafLevel(PlaylistNode parent, PlaylistNodePrimaryLeaf newLeaf1 , PlaylistNodePrimaryLeaf newLeaf2 ) {
		int ORDER = PlaylistNode.order;
		int MAX = ORDER * 2;
		int audioIdsToBePushedUp,j;

		ArrayList<Integer> audioIds1 = new ArrayList<>() ;
		ArrayList<Integer> audioIds2 = new ArrayList<>() ;
		int i=0;
		while (i<MAX) {
			if(newLeaf2.songAtIndex(0).audioId()>((PlaylistNodePrimaryIndex)parent).audioIdAtIndex(i)){
				i++;
				continue;
			}
			break;
		}
		if(i<ORDER) {
			j = 0;
			while (j<ORDER-1){audioIds1.add(((PlaylistNodePrimaryIndex)parent).audioIdAtIndex(j));j++;}
			audioIds1.add(i,newLeaf2.songAtIndex(0).audioId());
			while (j<MAX){audioIds2.add(((PlaylistNodePrimaryIndex)parent).audioIdAtIndex(j));j++;}
		}
		else{
			j = 0;
			while (j<ORDER){audioIds1.add(((PlaylistNodePrimaryIndex)parent).audioIdAtIndex(j));j++;}
			while (j<MAX){audioIds2.add(((PlaylistNodePrimaryIndex)parent).audioIdAtIndex(j));j++;}
			audioIds2.add(i-PlaylistNode.order , newLeaf2.songAtIndex(0).audioId());
		}
		audioIdsToBePushedUp = audioIds2.get(0);
		audioIds2.remove(0);
		ArrayList<PlaylistNode> children1 = new ArrayList<>() ;
		ArrayList<PlaylistNode> children2 = new ArrayList<>() ;
		if(i<ORDER) {
			for(j=0;j<ORDER;j++)children1.add(j,this.children.get(j)) ;
			children1.remove(i);
			children1.add(i,newLeaf1);
			children1.add(i+1,newLeaf2) ;
			for(;j<=MAX;j++)children2.add(j-ORDER, this.children.get(j));
		}
		else if(i==ORDER){
			for(j=0;j<ORDER;j++) children1.add(j,this.children.get(j));
			children1.add(j,newLeaf1);
			children2.add(0 , newLeaf2);
			j++;
			for(;j<=MAX; j++) children2.add( j-ORDER, this.children.get(j));
		}
		else{
			for(j=0;j<=ORDER;j++) children1.add(j,this.children.get(j) );
			for(;j<=MAX;j++) children2.add(j-ORDER-1,this.children.get(j));
			children2.remove(i-ORDER-1);
			children2.add(i-ORDER-1,newLeaf1);
			children2.add(i-ORDER,newLeaf2);
		}

		if (((PlaylistNodePrimaryIndex)parent).audioIdCount() > (2*MAX) && ((PlaylistNodePrimaryIndex)parent).getParent() != null){
			ArrayList<Integer> newRootAudioIds = new ArrayList<>();
			newRootAudioIds.add(audioIdsToBePushedUp);
			ArrayList<PlaylistNode> newRootChildren = new ArrayList<>();
			PlaylistNodePrimaryIndex newRoot = new PlaylistNodePrimaryIndex( null, newRootAudioIds ,newRootChildren);
			return newRoot;
		}
		else if(parent.getParent() == null) {
			ArrayList<Integer> newRootAudioIds = new ArrayList<>();
			newRootAudioIds.add(audioIdsToBePushedUp);
			ArrayList<PlaylistNode> newRootChildren = new ArrayList<>();
			PlaylistNodePrimaryIndex leftSplit = new PlaylistNodePrimaryIndex( null, audioIds1, children1);
			PlaylistNodePrimaryIndex rightSplit = new PlaylistNodePrimaryIndex( null, audioIds2, children2);
			newRootChildren.add(leftSplit);
			newRootChildren.add(rightSplit);
			PlaylistNodePrimaryIndex newRoot = new PlaylistNodePrimaryIndex( null, newRootAudioIds ,newRootChildren);
			leftSplit.setParent(newRoot);
			rightSplit.setParent(newRoot);
			j = 0; while (j<children2.size()){(children2.get(j)).setParent(rightSplit);j++;}
			j = 0; while (j<children1.size()){(children1.get(j)).setParent(leftSplit);j++;}
			return newRoot;
		}
		else {
			PlaylistNodePrimaryIndex parent2 = ((PlaylistNodePrimaryIndex)(parent.getParent()));
			PlaylistNodePrimaryIndex leftSplit = new PlaylistNodePrimaryIndex(null, audioIds1, children1);
			PlaylistNodePrimaryIndex rightSplit = new PlaylistNodePrimaryIndex(null, audioIds2, children2);
			j = 0; while (j<children1.size()){children1.get(j).setParent(leftSplit);j++;}
			j = 0; while (j < children2.size()){children2.get(j).setParent(rightSplit);j++;}
			if (parent2.audioIdCount() == MAX) return PushUpHigherLevels(audioIdsToBePushedUp, leftSplit, rightSplit, parent2);
			else{
				for(j = 0 ; j < parent2.audioIdCount() ; j++){
					if(audioIdsToBePushedUp > parent2.audioIdAtIndex(j)) continue;
					else break;
				}
				parent2.children.remove(j);
				parent2.audioIds.add(j, audioIdsToBePushedUp);
				parent2.children.add(j, leftSplit);
				parent2.children.add(j+1, rightSplit);
				leftSplit.setParent(parent2);
				rightSplit.setParent(parent2);
				return null ;
			}
		}
	}
	public PlaylistNode GetSuitableLeaf(Integer ID_TO_INSERT){
		PlaylistNode tracker = this;
		while (tracker.getType() != PlaylistNodeType.Leaf){
			int i = 0;
			while (true){
				if (((PlaylistNodePrimaryIndex)tracker).audioIdAtIndex(i) == -1) break;
				if (ID_TO_INSERT < ((PlaylistNodePrimaryIndex)tracker).audioIdAtIndex(i)) break;
				else i++;
			}
			tracker = ((PlaylistNodePrimaryIndex)tracker).getChildrenAt(i);
		}
		return tracker;
	}
}

