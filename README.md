GCube2 ver 0.2
======

絶賛開発途上です！！


##### GCubeとは

マルチプラットフォームで動作するモバイル向けフレームワークです。  
C/C++のワンソースでどの端末(iOS,Android,Tizen)でも動作するアプリが作成できます。  
OpenGLを使用しているので、主にゲーム向けとなります。  
基礎部分は[GBase](https://github.com/gclue/GBase "GBase")を使用しています。  


##### ライセンス

本体はMITライセンスになります。  
外部ライブラリに関してはそちらの指示に従ってください。


##### 現在出来る事

基本機能は[GBase](https://github.com/gclue/GBase "GBase")を参照してください。

拡張機能
* 行列計算
* ウィンドウ
* シーングラフ
    * シーン
    * カメラ
    * ライト
    * メッシュ
* 拡張可能なシェーダー


##### 使い方

プロジェクト作成用のシェルがありますので、そちらで新規プロジェクトを作成してください。

    ./createproject.sh project outdir
 
+   `project` : プロジェクト名
+   `outdir` : 出力フォルダパス



platformsフォルダ内に各OS用のプロジェクトファイルが入っています。  
mainフォルダのMainクラスがエントリポイントになりますので、ここに諸々記述してください。  
端末から発行される各種イベントはMainクラスからIApplicationEventListenerを通じて取得できます。  
GCubeConfig.hの記述を修正する事で各種設定が行えます。  
各種リソースはassetsフォルダ内に格納してください。  
Macでの開発を想定しています。  
Tizen向けビルドにシンボリックリンクを使用しているのでWindowsの場合はassetsフォルダを手動コピーしてください。


##### これから

オリジナルGCubeとは少し違った構成になると思います。

* イベント処理
* 2DUI
* 物理エンジン
* ボーンアニメーション
* 影
* スクリプト処理
* などなど。。。
