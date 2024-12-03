#include "SNewBlueprintView.h"
#include "Kismet2\KismetEditorUtilities.h"
#include "Kismet2\BlueprintEditorUtils.h"
#include "Kismet2\CompilerResultsLog.h"
#include "Kismet\KismetSystemLibrary.h"

SNewBlueprintView::~SNewBlueprintView() {
    if (UKismetSystemLibrary::IsValid(TempObject))
        TempObject->RemoveFromRoot();
}

void SNewBlueprintView::Construct(const FArguments& InArgs)
{
    PropertyOutputBox = SNew(SVerticalBox);
    PropertyInput = SNew(SEditableTextBox) // �༭������Property���л�����ַ���
        .Text(FText::FromString("(X=1.000000,Y=2.000000,Z=3.000000)"));

    this->ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        [
            SNew(SButton) // ��ť������½�һ����ͼ�࣬���ڴ洢Property
            .Text(FText::FromString("New Blueprint"))
            .OnClicked(this, &SNewBlueprintView::NewBlueprintClicked)
        ]
        + SVerticalBox::Slot()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            [
                PropertyInput.ToSharedRef()
            ]
            + SHorizontalBox::Slot()
            [
                SNew(SButton) // ��ť�������ȡ�༭���е��ַ������з����л����浽Property��
                .Text(FText::FromString("Save To Property"))
                .OnClicked(this, &SNewBlueprintView::SaveToProperty)
            ]
        ]
        + SVerticalBox::Slot()
        [
            SNew(SButton) // ��ť������Ƴ�/�������
            .Text(FText::FromString("Modify Property"))
            .OnClicked(this, &SNewBlueprintView::ModifyProperty)
        ]
        + SVerticalBox::Slot()
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            [
                SNew(SButton) // ��ť�����ͨ�����Ե���������UObject�����ԣ����л����ÿһ�����ԣ�UObject�������������ÿ���һ���ı���
                .Text(FText::FromString("Load Property"))
                .OnClicked(this, &SNewBlueprintView::LoadProperty)
            ]
            + SVerticalBox::Slot()
            [
                PropertyOutputBox.ToSharedRef()
            ]
        ]
    ];
}

void SNewBlueprintView::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

}

FReply SNewBlueprintView::NewBlueprintClicked()
{
    UClass* BlueprintClass = UBlueprint::StaticClass();
    UPackage* BlueprintOuter = GetTransientPackage();
    const FString DesiredName = FString::Printf(TEXT("EXPROUTCACHE_BP_%s"), *BlueprintClass->GetName());
    const FName BlueprintName = MakeUniqueObjectName(BlueprintOuter, BlueprintClass, FName(*DesiredName));
    BlueprintClass = FBlueprintEditorUtils::FindFirstNativeClass(BlueprintClass);

    UObject* Outer = GEditor;

    FName BlueprintCallingContext = "Blueprint Expression Evaluator";
    Blueprint = TSharedPtr<UBlueprint>(FKismetEditorUtilities::CreateBlueprint(UObject::StaticClass(), Outer, "Test", BPTYPE_Normal, BlueprintClass, UBlueprintGeneratedClass::StaticClass(), BlueprintCallingContext));
    FEdGraphPinType PinType;
    PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
    PinType.PinSubCategory = NAME_None;
    PinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();
    PinType.ContainerType = EPinContainerType::None;
    //PinType.PinValueType = ;
    FBlueprintEditorUtils::AddMemberVariable(Blueprint.Get(), "PropVec", PinType);

    FKismetEditorUtilities::CompileBlueprint(Blueprint.Get());

    TempObject = NewObject<UObject>(Outer, Blueprint->GeneratedClass);
    TempObject->AddToRoot();

    return FReply::Handled();
}

FReply SNewBlueprintView::SaveToProperty()
{
    FText ValueString = PropertyInput->GetText();
    TFieldIterator<FProperty> It(Blueprint->GeneratedClass);

    UObject* Object = TempObject;
    void* ValuePtr = It->ContainerPtrToValuePtr<void*>(Object);
    It->ImportText_Direct(*ValueString.ToString(), ValuePtr, Object, 0);
    return FReply::Handled();
}

FReply SNewBlueprintView::ModifyProperty()
{
    // �༭ͼ��ǰ��Ҫ���پɶ��󡣿�������/����ͼ���������ܱ༭ͼ������ʱ��������Object��

    // ���پɵĶ���
    TempObject->RemoveFromRoot();
    TempObject->MarkAsGarbage();

    FBlueprintEditorUtils::RemoveMemberVariable(Blueprint.Get(), "PropVec");

    FEdGraphPinType PinType;

    PinType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
    PinType.PinSubCategory = NAME_None;
    PinType.PinSubCategoryObject = nullptr; // TBaseStructure<bool>::Get();
    PinType.ContainerType = EPinContainerType::None;
    FBlueprintEditorUtils::AddMemberVariable(Blueprint.Get(), "PropBool", PinType);

    PinType.PinCategory = UEdGraphSchema_K2::PC_Int;
    PinType.PinSubCategory = NAME_None;
    PinType.PinSubCategoryObject = nullptr; // TBaseStructure<double>::Get();
    PinType.ContainerType = EPinContainerType::None;
    FBlueprintEditorUtils::AddMemberVariable(Blueprint.Get(), "PropInt", PinType);

    FCompilerResultsLog Log;
    FKismetEditorUtilities::CompileBlueprint(Blueprint.Get(), EBlueprintCompileOptions::None, &Log);

    FString Value;
    TempObject = NewObject<UObject>(GEditor, Blueprint->GeneratedClass);
    TempObject->AddToRoot();

    UObject* Object = TempObject;

    for (TFieldIterator<FProperty> It(Blueprint->GeneratedClass); It; ++It)
    {
        if (It->GetName() == "PropInt")
        {
            Value = "114514";
        }
        else if (It->GetName() == "PropBool")
        {
            Value = "True";
        }

        if (!Value.IsEmpty())
        {
            void* ValuePtr = It->ContainerPtrToValuePtr<void*>(Object);
            It->ImportText_Direct(*Value, ValuePtr, Object, 0);
        }
    }

    return FReply::Handled();
}

FReply SNewBlueprintView::LoadProperty()
{
    PropertyOutputBox->ClearChildren();
    for (TFieldIterator<FProperty> It(Blueprint->GeneratedClass); It; ++It)
    {
        UObject* Object = TempObject;
        void* ValuePtr = It->ContainerPtrToValuePtr<void*>(Object);
        FString Value;
        It->ExportTextItem_Direct(Value, ValuePtr, 0, Object, 0);
        auto Slot = PropertyOutputBox->AddSlot();
        auto ValueBox = SNew(STextBlock).Text(FText::FromString(Value));
        Slot.GetSlot()->AttachWidget(ValueBox);
    }

    return FReply::Handled();
}
