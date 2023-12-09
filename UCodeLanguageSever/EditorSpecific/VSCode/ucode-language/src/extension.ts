
import * as vscode from 'vscode';
import * as path from 'path';
import { workspace } from 'vscode';

import {
  LanguageClient,
  LanguageClientOptions,
  ServerOptions,
  StreamInfo,
  TransportKind
} from 'vscode-languageclient/node';

let client: LanguageClient;
export function activate(context: vscode.ExtensionContext) 
{
	
	let disposable = vscode.commands.registerCommand('uclang.update', () => 
	{
		vscode.window.showInformationMessage('build');
	});
	let disposable2 = vscode.commands.registerCommand('uclang.restart', () => 
	{
		client.stop();
		client.start();
	});
	let disposable3 = vscode.commands.registerCommand('uclang.start', () => 
	{
		client.start();
	});
	let disposable4 = vscode.commands.registerCommand('uclang.stop', () => 
	{
		client.stop();
	});

	context.subscriptions.push(disposable);
	context.subscriptions.push(disposable2);
	context.subscriptions.push(disposable3);
	context.subscriptions.push(disposable4);

	
	let serverOptions: ServerOptions = {
		command:"uclanglsp",
		args: ["start stdio"],
	};
	
  
	let clientOptions: LanguageClientOptions = {
	  documentSelector: [{ scheme: 'file', language: 'uclang' }],
	  synchronize: {
		fileEvents: workspace.createFileSystemWatcher('**/.uc')
	  },
	  
	  //outputChannelName: 'ULang Language Server',
	};
  
	client = new LanguageClient(
	  'languageServer',
	  'UCodeLanguage Server',
	  serverOptions,
	  clientOptions
	);
  
	client.start();
	vscode.window.showInformationMessage("lsp has started");
}

// This method is called when your extension is deactivated
export function deactivate() {}
