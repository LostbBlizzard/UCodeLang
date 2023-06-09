#pragma once

#include "LanguageSeverNameSpace.h"
#include "UCodeAnalyzer/Typedef.hpp"
#include "nlohmann/json.hpp"
#include <variant>
LanguageSeverStart

using namespace UCodeAnalyzer;
using json = nlohmann::json;

//from https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#responseMessage
//TypeScript definitions 
using integer = int;
using uinteger = int;

/**
 * Defines a decimal number. Since decimal numbers are very
 * rare in the language server specification we denote the
 * exact range with every decimal using the mathematics
 * interval notation (e.g. [0, 1] denotes all decimals d with
 * 0 <= d <= 1.
 */
using decimal = float;


//ts string
using string = String;

using UTF8 = char;
using UTF16 = Int32;
using UTF32 = int;


using DocumentUri = string;
using URI = string;
using boolean = bool;



//member?:T

template<typename T>
using TsOptional = std::optional<T>;


template<typename... T>
using TypePredicates = std::variant<T...>;

template<typename T>
using TsArray = std::vector<T>;

using unknown =bool;
using LSPAny = int;




enum class ErrorCodes : integer
{
    // Defined by JSON-RPC
    ParseError = -32700,
    InvalidRequest = -32600,
    MethodNotFound = -32601,
    InvalidParams = -32602,
    InternalError = -32603,


    ServerNotInitialized = -32002,
    UnknownErrorCode = -32001,
};

struct Position {
	/**
	 * Line position in a document (zero-based).
	 */
	uinteger line;

	/**
	 * Character offset on a line in a document (zero-based). The meaning of this
	 * offset is determined by the negotiated `PositionEncodingKind`.
	 *
	 * If the character value is greater than the line length it defaults back
	 * to the line length.
	 */
	uinteger character;
};
struct Range 
{
	/**
	 * The range's start position.
	 */
	Position start;

	/**
	 * The range's end position.
	 */
	Position end;
};
struct TextDocumentItem
{
	/**
	 * The text document's URI.
	 */
	DocumentUri uri;

	/**
	 * The text document's language identifier.
	 */
	string languageId;

	/**
	 * The version number of this document (it will increase after each
	 * change, including undo/redo).
	 */
	integer version;

	/**
	 * The content of the opened text document.
	 */
	string text;
};

struct TextDocumentIdentifier {
	/**
	 * The text document's URI.
	 */
	DocumentUri uri;
};
struct VersionedTextDocumentIdentifier : TextDocumentIdentifier
{
	/**
	 * The version number of this document.
	 *
	 * The version number of a document will increase after each change,
	 * including undo/redo. The number doesn't need to be consecutive.
	 */
	integer version;
};

struct  Location
{
	DocumentUri uri;
	Range range;
};

struct OptionalVersionedTextDocumentIdentifier : TextDocumentIdentifier {
	/**
	 * The version number of this document. If an optional versioned text document
	 * identifier is sent from the server to the client and the file is not
	 * open in the editor (the server has not received an open notification
	 * before) the server can send `null` to indicate that the version is
	 * known and the content on disk is the master (as specified with document
	 * content ownership).
	 *
	 * The version number of a document will increase after each change,
	 * including undo/redo. The number doesn't need to be consecutive.
	 */
	Optional<integer> version;
};

struct TextDocumentPositionParams {
	/**
	 * The text document.
	 */
	TextDocumentIdentifier textDocument;

	/**
	 * The position inside the text document.
	 */
	Position position;
};

enum class DiagnosticSeverity : integer
{/**
	 * Reports an error.
	 */
	Error = 1,
	/**
	 * Reports a warning.
	 */
	 Warning = 2,
	 /**
	  * Reports an information.
	  */
	  Information = 3,
	  /**
	   * Reports a hint.
	   */
	   Hint = 4,
};

enum class DiagnosticTag
{
	/**
	 * Unused or unnecessary code.
	 *
	 * Clients are allowed to render diagnostics with this tag faded out
	 * instead of having an error squiggle.
	 */
	Unnecessary = 1,
	/**
	 * Deprecated or obsolete code.
	 *
	 * Clients are allowed to rendered diagnostics with this tag strike through.
	 */
	 Deprecated = 2,

};

struct DiagnosticRelatedInformation {
	/**
	 * The location of this related diagnostic information.
	 */
	Location location;

	/**
	 * The message of this related diagnostic information.
	 */
	string message;
};

struct CodeDescription {
	/**
	 * An URI to open with more information about the diagnostic error.
	 */
	URI href;
};

struct Command
{
	/**
	 * Title of the command, like `save`.
	 */
	string title;
	/**
	 * The identifier of the actual command handler.
	 */
	string command;
	/**
	 * Arguments that the command handler should be
	 * invoked with.
	 */
	TsOptional<TsArray<LSPAny>> arguments;
};
struct TextEdit
{
	/**
	 * The range of the text document to be manipulated. To insert
	 * text into a document create a range where start === end.
	 */
	Range range;

	/**
	 * The string to be inserted. For delete operations use an
	 * empty string.
	 */
	string newText;
};

struct  ChangeAnnotation
{
	/**
	 * A human-readable string describing the actual change. The string
	 * is rendered prominent in the user interface.
	 */
	string label;

	/**
	 * A flag which indicates that user confirmation is needed
	 * before applying the change.
	 */
	TsOptional<boolean> needsConfirmation;

	/**
	 * A human-readable string which is rendered less prominent in
	 * the user interface.
	 */
	TsOptional<string> description;
};

/**
 * An identifier referring to a change annotation managed by a workspace
 * edit.
 *
 * @since 3.16.0.
 */
using ChangeAnnotationIdentifier = string;

struct  AnnotatedTextEdit :TextEdit
{
	/**
	 * The actual annotation identifier.
	 */
	ChangeAnnotationIdentifier annotationId;
};

struct  TextDocumentEdit
{
	/**
	 * The text document to change.
	 */
	OptionalVersionedTextDocumentIdentifier textDocument;

	/**
	 * The edits to be applied.
	 *
	 * @since 3.16.0 - support for AnnotatedTextEdit. This is guarded by the
	 * client capability `workspace.workspaceEdit.changeAnnotationSupport`
	 */
	TsArray<TypePredicates<TextEdit, AnnotatedTextEdit>> edits;
};



struct LocationLink
{

	/**
	 * Span of the origin of this link.
	 *
	 * Used as the underlined span for mouse interaction. Defaults to the word
	 * range at the mouse position.
	 */
	TsOptional<Range> originSelectionRange;

	/**
	 * The target resource identifier of this link.
	 */
	DocumentUri targetUri;

	/**
	 * The full target range of this link. If the target for example is a symbol
	 * then target range is the range enclosing this symbol not including
	 * leading/trailing whitespace but everything else like comments. This
	 * information is typically used to highlight the range in the editor.
	 */
	Range targetRange;

	/**
	 * The range that should be selected and revealed when this link is being
	 * followed, e.g the name of a function. Must be contained by the
	 * `targetRange`. See also `DocumentSymbol#range`
	 */
	Range targetSelectionRange;
};

struct  Diagnostic {
	/**
	 * The range at which the message applies.
	 */
	Range range;

	/**
	 * The diagnostic's severity. Can be omitted. If omitted it is up to the
	 * client to interpret diagnostics as error, warning, info or hint.
	 */
	TsOptional<DiagnosticSeverity> severity;

	/**
	 * The diagnostic's code, which might appear in the user interface.
	 */
	TsOptional<TypePredicates<integer, string>> code;

	/**
	 * An optional property to describe the error code.
	 *
	 * @since 3.16.0
	 */
	TsOptional<CodeDescription> codeDescription;

	/**
	 * A human-readable string describing the source of this
	 * diagnostic, e.g. 'typescript' or 'super lint'.
	 */
	TsOptional<string> source;

	/**
	 * The diagnostic's message.
	 */
	string message;

	/**
	 * Additional metadata about the diagnostic.
	 *
	 * @since 3.15.0
	 */
	TsOptional<TsArray<DiagnosticTag>> tags;

	/**
	 * An array of related diagnostic information, e.g. when symbol-names within
	 * a scope collide all definitions can be marked via this property.
	 */
	TsOptional<TsArray<DiagnosticRelatedInformation>> relatedInformation;

	/**
	 * A data entry field that is preserved between a
	 * `textDocument/publishDiagnostics` notification and
	 * `textDocument/codeAction` request.
	 *
	 * @since 3.16.0
	 */
	TsOptional<unknown> data;
};



using PositionEncodingKind = string;

/**
 * A set of predefined position encoding kinds.
 *
 * @since 3.17.0
 */
namespace PositionEncodingkind {

	/**
	 * Character offsets count UTF-8 code units (e.g bytes).
	 */
	PositionEncodingKind PositionEncodingKind8 = "utf-8";

	/**
	 * Character offsets count UTF-16 code units.
	 *
	 * This is the default and must always be supported
	 * by servers
	 */
	PositionEncodingKind PositionEncodingKind16 = "utf-16";

	/**
	 * Character offsets count UTF-32 code units.
	 *
	 * Implementation note: these are the same as Unicode code points,
	 * so this `PositionEncodingKind` may also be used for an
	 * encoding-agnostic representation of character offsets.
	 */
	PositionEncodingKind PositionEncodingKind32 = "utf-32";
};

//https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#serverCapabilities
struct ServerCapabilities {

	/**
	 * The position encoding the server picked from the encodings offered
	 * by the client via the client capability `general.positionEncodings`.
	 *
	 * If the client didn't provide any position encodings the only valid
	 * value that a server can return is 'utf-16'.
	 *
	 * If omitted it defaults to 'utf-16'.
	 *
	 * @since 3.17.0
	 */
	TsOptional<PositionEncodingKind> positionEncoding;
};

struct ResponseError {
	/**
	 * A number indicating the error type that occurred.
	 */
	integer code;

	/**
	 * A string providing a short description of the error.
	 */
	string message;

	/**
	 * A primitive or structured value that contains additional
	 * information about the error. Can be omitted.
	 */
	TsOptional<json> data;
};

struct InitializeResult
{
	/**
	 * The capabilities the language server provides.
	 */
	ServerCapabilities capabilities;

	/**
	 * Information about the server.
	 *
	 * @since 3.15.0
	 */
	struct Struct
	{
		string name;
		TsOptional<string> version;
	};

	TsOptional< Struct> serverInfo;
};

LanguageSeverEnd

