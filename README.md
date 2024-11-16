# DTCreator

- 能遍历指定文件夹下的所有 assert 然后按照指定的规则生成对应的 DataTable

- 用于创建收集类型的 DataTable: 例如 DT_UI, DT_AbilityTraits 等

# 使用方式

1. 在项目启动的时候自动生成

2. 在命令行调用 CreateDataTable

3. 点击界面工具栏的 DataTable 图标

#配置说明

1. 根据创建工具类，继承自 DTCreatorHelperBase

2. 重写 MakeDataTableRow 函数，通过入参传入的 FileObject 创建相应的 DataTableRowStruct

3. 将创建好的 Struct 添加至 DataTable

	3.1 C++创建的 FTableRowBase 可直接调用引擎原生的 UDataTableFunctionLibrary::AddDataTableRow 函数
	
	3.2蓝图创建的 UserDefineStruct 则需要调用 UDTCreatorFunctionLibrary::AddDataTableRowByUserDefinedStruct 函数
	
4. 在编辑器界面打开 Edit->ProjectSettings，找到 Game->DTCreatorSetting, 添加相应的配置

		FString CollectFolderPath; 从哪儿开始收集 Asset
		例：/Game/Cooperation/DataTable/Ability/AbilityDataAsset

		FString ExportFolderPath; DataTable 生成到哪个文件夹
		例：/Game/Generated/DataTable

		FString ExportFileName; DataTable 的名称
		例：DT_TraitDataAssetInfo

		TObjectPtr<class UScriptStruct> Struct; DataTable 的 Struct
		例：ST_TraitDataAssetInfo

		TSubclassOf<UDTCreatorHelperBase> HelperClass; 工具类
		例：BP_DTCreatorHelper_AbilityTrait
