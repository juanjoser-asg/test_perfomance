
	function ModalConstants()
	{
		this.FunctionNop = "function(){}";
	}
	function ModalExceptionMessages()
	{
		this.BaseMessage = "Modal Exception";
		this.CreationException = this.BaseMessage+"Error in Creating an Instance ";
		this.TypeNotSupplied = this.CreationException+" Type Not Supplied";
		this.AbstractMethodException = this.BaseMessage+"Called Abstract Method Must Override";
		this.ObjectIsNotAMemberException = this.BaseMessage+"Adding Object is Not a Modal Member";
		this.ObjectIsNotAColumnException = this.BaseMessage+"Adding Object is Not a Valid Column";
		this.DuplicatedGroupExceptionMessage=function(sGroupId)
		{
			return(this.CreationException+"Group with id --["+sGroupId+"]-- duplicated ");
		}
	}
	function ModalMember()
	{
		var oConstants = new ModalConstants();
		var oExceptions = new ModalExceptionMessages();
		this._getConstants=function()
		{
			return(oConstants);

        }
        this.GetType = function() {
            return "Modal Member";
        }
		this._getExceptions=function()
		{
			return(oExceptions);
		}
		this.GetParentType=function()
		{
			return("Modal Member");
		}
		this.Process=function(oReference)
		{
			throw(new Error(this._getExceptions().AbstractMethodException));
		}
	} 
	

	function ModalButton(sType,sName,sFloat,sText,sClick,sUrl,sParams,sOnSuccess,sOnMouseUp,sOnMouseDown,sOnMouseOver,sOnMouseOut,sOnError)
	{
		var baseConstructor=ModalMember;
		baseConstructor.call(this);
		this.type=sType;
		this.name=sName;
		this.floatCSS=sFloat;
		this.text=sText;
		this.onClick=sClick;
		if(!sOnMouseUp || sOnMouseUp=="")this.onMouseUp=this._getConstants().FunctionNop;
		if(!sOnMouseDown || sOnMouseDown=="")this.onMouseDown=this._getConstants().FunctionNop;
		if(!sOnMouseOver || sOnMouseOver=="")this.onMouseOver=this._getConstants().FunctionNop;
		if(!sOnMouseOut || sOnMouseOut=="")this.onMouseOut=this._getConstants().FunctionNop;
		this.url=sUrl;
		this.parameters=sParams;
		if(!sOnSuccess||sOnSuccess==undefined||sOnSuccess=="undefined")sOnSuccess="";
		this.onSuccess=sOnSuccess;
		if(sOnError!=""||sOnError!=undefined||sOnError!="undefined")this.onError=sOnError;
		
		this.AddProp=function(sPropName,sPropValue)
		{
			eval("this."+sPropName+"=\""+sPropValue+"\"");
			return(this);
		}
		
		this.Process=function(oReference)
		{
			if(!oReference.buttons)oReference.buttons=new Array();
			oReference.buttons.push(this);
		}
	}	
	function ModalObject(sTitle,sSubTitle)
	{
		var oExceptions = new ModalExceptionMessages();
		var groupList = new Array();
		var _self = this;
		var title=sTitle;
		var subTitle=sSubTitle;
		
		this._checkGroupExists=function(sId)
		{
			if(groupList[sId])throw(new Error(oExceptions.DuplicatedGroupExceptionMessage(sId)));
		}
		this._addGroupToGroupList=function(oGroup)
		{
			groupList[oGroup.id]=oGroup;
		}
		this.AddObject=function(oObject,sRoot)
		{
			sElement=getRoot(sRoot);
			if(oObject.GetParentType()!="Modal Member")throw(new Error(this._getExceptions().ObjectIsNotAMemberException));
			oObject.Process(eval("this."+sElement),this);
			return(this);	
		}
		this.AddObjectToRoot=function(oObject)
		{
		    oObject.Process(this,this);
		    return(this);
		}
		this.AddPropToRoot=function(sPropName,sPropValue)
		{
			eval("this."+sPropName+"=\""+sPropValue+"\"");
			return(this);
		}
		this.AddProp=function(sPropName,sPropValue,sRoot)
		{
			sElement=getRoot(sRoot);
			var escaped = new ModalStringHelper().EscapeQuotes(sPropValue);
			eval("this."+sElement+"."+sPropName+"=\""+escaped+"\"");
			
			return(this);
		}
		function getRoot(sRoot)
		{
			if(!sRoot||sRoot=="undefined"||sRoot=="")sRoot="params";
			if(eval("!_self."+sRoot))eval("_self."+sRoot+"= new ModalParam(\""+title+"\",\""+subTitle+"\")");
			return(sRoot);
		}
		this.AddPropObject=function(sPropName,oObject,sRoot)
		{
			sElement=getRoot(sRoot);
			eval("this."+sElement+"."+sPropName+"="+JSON.stringify(oObject));
			return(this);
		}
	}
	function CreateModal(sTitle,sSubTitle)
	{
		var oModal = new ModalObject(sTitle,sSubTitle);
		return(oModal);
	}
	function ModalParam(sTitle,sSubTitle)
	{
		if(sTitle&&sTitle!="undefined"&&sTitle!="")this.modalTitle=sTitle;
		if(sSubTitle&&sSubTitle!="undefined"&&sSubTitle!="")this.subtitle=sSubTitle;
	}
    function ModalField(sName,sId,sClass,sInfoMessage,sErrorMessage)
	{
		var baseConstructor = ModalMember;
		baseConstructor.call(this);
		
		this.name = sName;
		this.id = sId;
		this.inputClass = sClass;
		this.infoMessage = sInfoMessage;
		this.errorMessage = sErrorMessage;
	
		this.Process=function(oReference)
		{
			if(!oReference.fields)oReference.fields=new Array();
			oReference.fields.push(this);

        }
        this.GetType = function() {
            return "Modal Field";
        }
		this.AddProp=function(sPropName,sPropValue)
		{
			var propValue = new String("" + sPropValue);
			var escaped = new ModalStringHelper().EscapeQuotes(sPropValue)
	
			eval("this."+sPropName+"=\""+escaped+"\"");
			return this;
		}
		this.AddObject=function(sPropName,oObject)
		{
			eval("this."+sPropName+"="+JSON.stringify(oObject));
			return this;
		}
		this.AddCorrectMatchValidation=function(oJSONModalField)
		{
			var oCorrectMatch={ "correctMatch":oJSONModalField.id};
			this.AddObject("validations",oCorrectMatch);
			return this;
		}
    this.AddValidation=function(oObject)
    {
        if(oObject.GetBaseType()!="ModalValidations") throw(new Error("not a validation"));
        oObject.Process(this);
        return this;
    }
		
	}


	function ModalHiddenField(sName,sId,sClass,sInfoMessage,sErrorMessage)
	{
		var baseConstructor = ModalField;
		baseConstructor.call(this,sName,sId,sClass,sInfoMessage,sErrorMessage);
		this.type="hidden";
	}
  function ModalValidations()
  {
      var baseType = "ModalValidations";
      this._fieldName;
      this._fieldValue;
      this.GetBaseType = function()
      {
          return(baseType);
      }
      this.Process=function(oReference)
      {
        if(!oReference.validations)oReference.validations=new Object();
        oReference.validations[this._fieldName]=this._fieldValue;
      }
  }
  function ModalStringHelper()
  {
	  this.EscapeQuotes=function(input)
	  {
		  var propValue = new String("" + input);
		  var escaped = propValue.replace(/"/ig,'\\"');
		  return escaped;	
	  }
  }
  
  