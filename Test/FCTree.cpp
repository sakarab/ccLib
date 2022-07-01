//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FCTree.h"
#include "ccUtils.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
Data::Data()
	: type(unknown), FName(EmptyStr)
{
}

Data::Data( const Data& src )
	: type(src.type), FName(src.FName)
{
}

Data::Data( FileType a, const AnsiString& b )
	: type(a), FName(b)
{
}

Data::~Data()
{
}

Data& Data::operator=( const Data& src )
{
	if ( this != &src )
	{
		type = src.type;
		FName = src.FName;
	}
	return ( *this );
}
//---------------------------------------------------------------------------
__fastcall TfrmCTree::TfrmCTree(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfrmCTree::SelectDir()
{
	AnsiString	result( OpenDlg->FileName );

	if ( result.IsEmpty() )
		OpenDlg->FileName = "Filename will not be used";
	if ( !OpenDlg->Execute() )
		result = EmptyStr;
	else
		result = ExtractFilePath( OpenDlg->FileName );
	return ( result );
}
//---------------------------------------------------------------------------
bool TfrmCTree::OnFindFile( vcl::TScanDir *scan_dir )
{
	FileTree	*tree = reinterpret_cast<FileTree *>(scan_dir->Data);

	tree->add( new Data( Data::file, scan_dir->LongFileName ) );
	return ( true );
}
//---------------------------------------------------------------------------
vcl::TScanDir::TFindDirResult TfrmCTree::OnFindDir( vcl::TScanDir *scan_dir )
{
	FileTree			*tree = reinterpret_cast<FileTree *>(scan_dir->Data);

	if ( scan_dir->LongFileName[1] != '.' )
		scan_dir->Data = &(*(tree->add( new Data( Data::dir, scan_dir->LongFileName ) )));
	return ( vcl::TScanDir::sdfrContinue );
}
//---------------------------------------------------------------------------
bool TfrmCTree::OnDirExited( vcl::TScanDir *scan_dir )
{
	FileTree	*tree = reinterpret_cast<FileTree *>(scan_dir->Data);

	scan_dir->Data = tree->parent();
	return ( true );
}
//---------------------------------------------------------------------------
void __fastcall FillTreeView1( TTreeView *tvTree, TTreeNode *tv_parent, FileTree *tree_parent )
{
    if ( ! tree_parent->has_children() )
        return;

    FileTree::node  tree_node = tree_parent->begin();
    while ( tree_node != tree_parent->end() )
    {
        TTreeNode   *node = tvTree->Items->AddChild( tv_parent, tree_node->get_value()->GetName() );

        FillTreeView1( tvTree, node, &(*tree_node) );
        ++tree_node;
    }
}

void __fastcall TfrmCTree::FillTreeView( FileTree& file_tree )
{
    vcl::UpdateLocker<TTreeNodes>    aa( tvTree->Items );

    FillTreeView1( tvTree, NULL, &file_tree );
/*
    tvTree->Items->BeginUpdate();
    try
    {
        FillTreeView1( tvTree, NULL, &file_tree );
    }
    __finally
    {
        tvTree->Items->EndUpdate();
    }
*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmCTree::btnCTreeClick(TObject *Sender)
{
	vcl::TScanDir	scan_dir;
	AnsiString		dir( SelectDir() );

	if ( dir.IsEmpty() )
		return;
	scan_dir.OnFindFile = OnFindFile;
	scan_dir.OnFindDir = OnFindDir;
	scan_dir.OnDirExited = OnDirExited;
	scan_dir.FileSpecList.push_back( "*.*" );

	FileTree		file_tree( NULL, new Data( Data::dir, dir ) );

	scan_dir.Data = &file_tree;
	scan_dir.Run( dir, true );
    FillTreeView( file_tree );
}
//---------------------------------------------------------------------------

