
import * as vscode from 'vscode';
import * as path from 'path';
import { workspace, ExtensionContext } from 'vscode';

import {
  LanguageClient,
  LanguageClientOptions,
  ServerOptions,
  TransportKind
} from 'vscode-languageclient/node';

let client: LanguageClient;
export function activate(context: vscode.ExtensionContext) 
{
	console.log('Congratulations, your extension "uclang" is now active!');

	let disposable = vscode.commands.registerCommand('uclang.update', () => 
	{
		vscode.window.showInformationMessage('build');
	});
	let disposable2 = vscode.commands.registerCommand('uclang.restart', () => 
	{
		vscode.window.showInformationMessage('restart');
	});
	let disposable3 = vscode.commands.registerCommand('uclang.start', () => 
	{
		vscode.window.showInformationMessage('start');
	});
	let disposable4 = vscode.commands.registerCommand('uclang.stop', () => 
	{
		vscode.window.showInformationMessage('stop');
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
	  }
	};
  
	client = new LanguageClient(
	  'languageServerExample',
	  'Language Server Example',
	  serverOptions,
	  clientOptions
	);
  
	client.start();
}

// This method is called when your extension is deactivated
export function deactivate() {}
