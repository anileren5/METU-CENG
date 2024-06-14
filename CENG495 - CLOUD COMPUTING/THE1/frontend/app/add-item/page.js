"use client";

import { useState } from 'react';
import Body from "@/components/Body";
import Navbar from "@/components/Navbar";
import Typography from "@mui/material/Typography";
import FormControl from '@mui/material/FormControl';
import Select from '@mui/material/Select';
import MenuItem from '@mui/material/MenuItem';
import InputLabel from '@mui/material/InputLabel';
import Button from '@mui/material/Button';
import TextField from '@mui/material/TextField';
import {useUser} from "@auth0/nextjs-auth0/client";
import {addItem} from "@/utils/api/services";
import { Snackbar } from '@material-ui/core';
import { green } from '@material-ui/core/colors';
import CloseIcon from '@material-ui/icons/Close';
import {Checkbox, FormControlLabel, SnackbarContent} from "@mui/material";
import IconButton from "@mui/material/IconButton";
import DeleteIcon from '@material-ui/icons/Delete';

export default function AddItemPage() {
    const { user } = useUser();
    const [category, setCategory] = useState('');
    const [image, setImage] = useState('');
    const [imageName, setImageName] = useState('');
    const [nestedFields, setNestedFields] = useState([]);
    const [flatFields, setFlatFields] = useState([]);
    const [description, setDescription] = useState('');
    const [title, setTitle] = useState('');
    const [price, setPrice] = useState('');
    const [currency, setCurrency] = useState('');
    const [successMessage, setSuccessMessage] = useState(false);
    const [errorMessage, setErrorMessage] = useState(false);
    const [listFields, setListFields] = useState([]);
    const [shareContact, setShareContact] = useState(false);


    const handleAddListField = () => {
        const newListField = { name: '', list: [{ value: '' }] };
        setListFields([...listFields, newListField]);
    };

    const handleAddListItem = (index) => {
        const updatedFields = [...listFields];
        updatedFields[index].list.push({ value: '' });
        setListFields(updatedFields);
    };

    const handleListNameChange = (index, value) => {
        const updatedFields = [...listFields];
        updatedFields[index].name = value;
        setListFields(updatedFields);
    };

    const handleListItemValueChange = (listIndex, itemIndex, value) => {
        const updatedFields = [...listFields];
        updatedFields[listIndex].list[itemIndex].value = value;
        setListFields(updatedFields);
    };

    const handleDeleteFlatField = (index) => {
        const updatedFlatFields = [...flatFields];
        updatedFlatFields.splice(index, 1); // Remove the field at the specified index
        setFlatFields(updatedFlatFields); // Update the state with the modified flat fields
    };

    const handleDeleteNestedForm = (nestedIndex) => {
        const updatedNestedFields = [...nestedFields];
        updatedNestedFields.splice(nestedIndex, 1); // Remove the subform at the specified index
        setNestedFields(updatedNestedFields); // Update the state with the modified nested fields
    };

    const handleDeleteNestedField = (nestedIndex, fieldIndex) => {
        const updatedNestedFields = [...nestedFields];
        updatedNestedFields[nestedIndex].fields.splice(fieldIndex, 1); // Remove the field at the specified index
        setNestedFields(updatedNestedFields); // Update the state with the modified nested fields
    };

    const handleCurrencyChange = (event) => {
        setCurrency(event.target.value);
    };

    const handleTitleChange = (event) => {
        setTitle(event.target.value);
    };

    const handlePriceChange = (event) => {
        setPrice(event.target.value);
    };

    const handleCategoryChange = (event) => {
        setCategory(event.target.value);
    };

    const handleImageChange = (event) => {
        const file = event.target.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onloadend = () => {
                setImage(reader.result);
            };
            reader.readAsDataURL(file);
            setImageName(file.name);
        }
    };

    const handleNestedNameChange = (index, value) => {
        const updatedFields = [...nestedFields];
        updatedFields[index].name = value;
        setNestedFields(updatedFields);
    };

    const handleFieldNameChange = (nestedIndex, fieldIndex, value) => {
        const updatedFields = [...nestedFields];
        updatedFields[nestedIndex].fields[fieldIndex].fieldName = value;
        setNestedFields(updatedFields);
    };

    const handleFieldValueChange = (nestedIndex, fieldIndex, value) => {
        const updatedFields = [...nestedFields];
        updatedFields[nestedIndex].fields[fieldIndex].fieldValue = value;
        setNestedFields(updatedFields);
    };

    const handleAddFieldToNested = (index) => {
        const updatedFields = [...nestedFields];
        updatedFields[index].fields.push({ fieldName: '', fieldValue: '' });
        setNestedFields(updatedFields);
    };

    const handleAddNestedField = () => {
        setNestedFields([...nestedFields, { name: '', fields: [{ fieldName: '', fieldValue: '' }] }]);
    };

    const handleFlatFieldNameChange = (index, value) => {
        const updatedFields = [...flatFields];
        updatedFields[index].fieldName = value;
        setFlatFields(updatedFields);
    };

    const handleFlatFieldValueChange = (index, value) => {
        const updatedFields = [...flatFields];
        updatedFields[index].fieldValue = value;
        setFlatFields(updatedFields);
    };

    const handleAddFlatField = () => {
        setFlatFields([...flatFields, { fieldName: '', fieldValue: '' }]);
    };

    const handleDescriptionChange = (event) => {
        setDescription(event.target.value);
    };

    const handleCloseSuccess = () => {
        setSuccessMessage(false);
    };

    const handleCloseError = () => {
        setErrorMessage(false);
    };


    const handleSubmit = async (event) => {
        event.preventDefault();
        let item = {
            user_id: user.sub,
            category: category,
            image: image,
            flatFields: flatFields,
            nestedFields: nestedFields,
            listFields: listFields,
            description: description,
            creation_date: new Date().toISOString(),
            update_date: new Date().toISOString(),
            title: title,
            price: price,
            currency: currency,
            priceInTermsOfTL: currency === "TL" ? price : (currency === "USA" ? 32 * price : 35 * price),
            isActive: true,
            shareContact: shareContact
        }
        let response = await addItem(item);
        if (response.message === "Success") {
            setSuccessMessage(true);
        } else {
            setErrorMessage(true);
        }
        setCategory("");
        setImage("");
        setImageName("");
        setNestedFields([]);
        setFlatFields([]);
        setDescription("");
        setTitle("");
        setPrice("");
        setCurrency("");
        setListFields([]);
    };

    const handleCheckboxChange = (event) => {
        setShareContact(!shareContact);
    };

    return (
        <Body singleRow>

            <Navbar />
            <Typography variant="h3" align="center" style={{ marginBottom: '20px' }}>New Item Form</Typography>
            <form onSubmit={handleSubmit} style={{display: 'flex', flexDirection: 'column', alignItems: 'center'}}>
                <FormControl fullWidth style={{marginBottom: '20px', width: '80%'}}>
                    <InputLabel>Select Category</InputLabel>
                    <Select
                        value={category}
                        onChange={handleCategoryChange}
                        style={{width: '100%'}}
                    >
                        <MenuItem value="vehicle">Vehicles</MenuItem>
                        <MenuItem value="computer">Computer</MenuItem>
                        <MenuItem value="phone">Phone</MenuItem>
                        <MenuItem value="private lesson">Private Lesson</MenuItem>
                    </Select>
                </FormControl>
                <FormControlLabel
                    control={<Checkbox checked={shareContact} onChange={handleCheckboxChange} />}
                    label="Share owner contact details with Regular (Not Authorized) users"
                />
                <div style={{
                    display: 'flex',
                    justifyContent: 'center',
                    alignItems: 'center',
                    marginBottom: '20px',
                    width: '80%'
                }}>
                    <label htmlFor="upload-button">
                        <input
                            id="upload-button"
                            type="file"
                            accept="image/*"
                            onChange={handleImageChange}
                            style={{display: 'none'}}
                        />
                        <Button component="span" variant="outlined" style={{marginRight: '10px'}}>
                            Upload Photograph
                        </Button>
                    </label>
                    {imageName && <Typography variant="subtitle1">{imageName}</Typography>}
                </div>
                <TextField
                    label="Title"
                    value={title}
                    onChange={handleTitleChange}
                    fullWidth
                    style={{marginBottom: '20px', width: '80%'}}
                />
                <div style={{display: 'flex', width: '80%', marginBottom: '20px'}}>
                    <TextField
                        label="Price"
                        type="number"
                        value={price}
                        onChange={handlePriceChange}
                        style={{width: '50%', marginRight: '10px'}}
                    />
                    <FormControl style={{width: '50%'}}>
                        <InputLabel>Currency</InputLabel>
                        <Select
                            value={currency}
                            onChange={handleCurrencyChange}
                        >
                            <MenuItem value="TL">TL</MenuItem>
                            <MenuItem value="USD">USD</MenuItem>
                            <MenuItem value="EUR">EUR</MenuItem>
                            {/* Add more currencies as needed */}
                        </Select>
                    </FormControl>
                </div>
                <Typography variant="h6" style={{marginBottom: '20px', width: '80%', textAlign: 'center'}}>Detailed
                    Properties</Typography>
                {nestedFields.map((nested, nestedIndex) => (
                    <div key={nestedIndex}
                         style={{
                             border: '1px solid #ccc',
                             padding: '10px',
                             marginBottom: '20px',
                             width: '80%',
                             position: 'relative'
                         }}
                    >
                        <TextField
                            label="Detailed Property Name"
                            value={nested.name || ''}
                            onChange={(event) => handleNestedNameChange(nestedIndex, event.target.value)}
                            style={{marginBottom: '10px', width: '100%'}}
                        />
                        {nested.fields.map((field, fieldIndex) => (
                            <div key={fieldIndex} style={{display: 'flex', marginBottom: '10px'}}>
                                <TextField
                                    label="Field Name"
                                    value={field.fieldName || ''}
                                    onChange={(event) => handleFieldNameChange(nestedIndex, fieldIndex, event.target.value)}
                                    style={{marginRight: '5px', width: '50%'}}
                                />
                                <TextField
                                    label="Field Value"
                                    value={field.fieldValue || ''}
                                    onChange={(event) => handleFieldValueChange(nestedIndex, fieldIndex, event.target.value)}
                                    style={{marginLeft: '5px', width: '50%'}}
                                />
                                <IconButton aria-label="delete" onClick={() => handleDeleteNestedField(nestedIndex, fieldIndex)}>
                                    <DeleteIcon />
                                </IconButton>
                            </div>
                        ))}
                        <div style={{ textAlign: 'center' }}>
                            <Button
                                variant="outlined"
                                onClick={() => handleAddFieldToNested(nestedIndex)}
                                style={{marginBottom: '10px'}}
                            >
                                Add Simple Property
                            </Button>
                        </div>
                    </div>
                ))}
                <Button variant="outlined" onClick={handleAddNestedField} style={{marginBottom: '20px', width: '80%'}}>
                    Add Detailed Property
                </Button>
                <Typography variant="h6" style={{marginBottom: '20px', width: '80%', textAlign: 'center'}}>Simple
                    Properties</Typography>
                {flatFields.map((field, index) => (
                    <div key={index} style={{display: 'flex', marginBottom: '10px', width: '80%'}}>
                        <TextField
                            label="Field Name"
                            value={field.fieldName || ''}
                            onChange={(event) => handleFlatFieldNameChange(index, event.target.value)}
                            style={{marginRight: '5px', width: '50%'}}
                        />
                        <TextField
                            label="Field Value"
                            value={field.fieldValue || ''}
                            onChange={(event) => handleFlatFieldValueChange(index, event.target.value)}
                            style={{marginLeft: '5px', width: '50%'}}
                        />
                        <IconButton aria-label="delete" onClick={() => handleDeleteFlatField(index)}>
                            <DeleteIcon />
                        </IconButton>
                    </div>
                ))}
                <Button variant="outlined" onClick={handleAddFlatField} style={{marginBottom: '20px', width: '80%'}}>
                    Add Simple Property
                </Button>
                <TextField
                    label="Description"
                    multiline
                    rows={4}
                    variant="outlined"
                    value={description}
                    onChange={handleDescriptionChange}
                    fullWidth
                    style={{marginBottom: '20px', width: '80%'}}
                />
                {listFields.map((list, index) => (
                    <div key={index} style={{ border: '1px solid #ccc', padding: '10px', marginBottom: '20px', width: '80%' }}>
                        <div style={{ marginBottom: '10px', width: '100%' }}>
                            <TextField
                                label="List Property Name"
                                value={list.name}
                                onChange={(event) => handleListNameChange(index, event.target.value)}
                                fullWidth
                            />
                        </div>
                        {list.list.map((item, itemIndex) => (
                            <div key={itemIndex} style={{ marginTop: '10px', width: '100%' }}>
                                <TextField
                                    label={`List Property Item ${itemIndex + 1}`}
                                    value={item.value}
                                    onChange={(event) => handleListItemValueChange(index, itemIndex, event.target.value)}
                                    fullWidth
                                />
                            </div>
                        ))}
                        <Button variant="outlined" onClick={() => handleAddListItem(index)} style={{ marginTop: '10px' }}>Add List Item</Button>
                    </div>
                ))}
                <Button variant="outlined" onClick={handleAddListField} style={{ marginBottom: '20px', width: '80%' }}>
                    Add List Property
                </Button>
                <div style={{display: 'flex', justifyContent: 'center', width: '80%'}}>
                    <Button type="submit" variant="contained" color="primary" style={{width: '48%'}}>
                        Submit
                    </Button>
                </div>
                <Snackbar
                    anchorOrigin={{ vertical: 'bottom', horizontal: 'center' }}
                    open={successMessage || errorMessage}
                    autoHideDuration={2000}
                    onClose={successMessage ? handleCloseSuccess : handleCloseError}
                >
                    <SnackbarContent
                        style={{ backgroundColor: successMessage ? green[600] : green[600] }}
                        message={successMessage ? "Successfully added!" : "Error: Something went wrong!"}
                        action={
                            <IconButton size="small" aria-label="close" color="inherit" onClick={successMessage ? handleCloseSuccess : handleCloseError}>
                                <CloseIcon fontSize="small" />
                            </IconButton>
                        }
                    />
                </Snackbar>
            </form>
        </Body>
    );
}
